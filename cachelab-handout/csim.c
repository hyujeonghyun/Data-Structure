#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachelab.h"
#include <unistd.h>

typedef unsigned long long int Address;

typedef struct line{
    int valid;
    int tag;
    int last_time;
} Block;

typedef struct set{
    Block* blocks;
} Set;

typedef struct cache{
    Set* sets;
    int set_num; 
    int block_num;
} Cache;

Cache cache;
int block_bits;
int first_time = 1;

int s,E,b,v;
char* t;

int hits = 0;
int misses = 0;
int evictions = 0;

// trace 파일 내용을 실행하는 함수
void execute_file(FILE* trace){
    int cmd;
    Address address;
    int size;

    while(fscanf(trace, " %d %llx,%d", &cmd, &address, &size) != EOF){      // " %d"이렇게 띄어쓰면 앞에 들어간 공백 문자 무시
        switch (cmd){
            case 'L':
                cache_access(address);
                break;
            case 'M':
                cache_access(address);
                cache_access(address);
                break;
            case 'S':
                cache_access(address);
                break;
            default:
                break;
        }
    }
}

void cache_access(Address address){
    Address tag = address >> (s+b);     // s+b 만큼 잘라버리기.
    Address set_index = (address - (tag << (s+b))) >> b;    // address에서 tag부분 없애고 b 부분 자르기

    Set* select_set = &cache.sets[set_index];

    for(int i = 0; i < cache.block_num; i++){
        if(select_set->blocks[i].valid){
            if(select_set->blocks[i].tag != tag) {continue;}
            
            select_set->blocks[i].last_time = first_time++;
            hits++;
            return;
        }    
    }
    
    misses++;
    unsigned long long int min = select_set->blocks[0].last_time;
    int old_block = 0;
    for(int i = 1; i < cache.block_num; i++){
        if(select_set->blocks[i].last_time < min){
            old_block = i;
            min = select_set->blocks[i].last_time;
        }
    }

    if(select_set->blocks[old_block].valid){
        evictions++;
    }
    select_set->blocks[old_block].tag = tag;
    select_set->blocks[old_block].valid = 1;
    select_set->blocks[old_block].last_time = first_time++;
}



int main(int argc, char argv[])
{
    int cmd;
    FILE* file;

    // 입력 받기
    while(cmd = getopt(argc, argv, "vs:E:b:t:") != EOF){
        switch (cmd)
        {
        case 'v': 
            v = 1;
            break;
        case 's':
            s = atoi(optarg);
            cache.set_num = 1 << s;
            break;
        case 'E':
            E = atoi(optarg);
            cache.block_num = E;
            break;
        case 'b':
            b = atoi(optarg);
            block_bits = b;
            break;
        case 't':
            t = optarg;
            break;
        default:
            exit(1);
        }
    }

    // block수와 block bit수가 0일 수 없으므로...
    if(!(E && b)) {
        exit(1);
    }

    // t옵션을 통해 받은 파일 열기
    file = fopen(t, "r");
    
    cache.sets = malloc(sizeof(Set)*cache.set_num);
    for(int i = 0; i < cache.set_num; i++){
        // calloc 쓰면 메모리 할당되는 곳을 0으로 초기화 시켜주기 때문에 변수들도 0으로 초기화됨. calloc(n, size)
        cache.sets[i].blocks = calloc(cache.block_num, sizeof(Block));
    }

    execute_file(file);

    printSummary(hits, misses, evictions);
    fclose(file);
    return 0;
}
