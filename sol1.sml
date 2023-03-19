(*#1 merge list*)
fun merge(xs:int list, ys: int list) : int list = 
  if (null xs) then ys
  else if (null ys) then xs
  else (hd xs)::merge(tl xs, ys)


(*#2 reverse list*)
fun reverse(xs:int list) : int list =
let
  fun rev_aux(ys:int list, zs:int list) : int list =
    if (null ys) then zs
    else rev_aux(tl ys, (hd ys)::zs)

in
  rev_aux(xs, [])
end


(*#3 Pi function*)
fun pi(a:int, b:int, f:int->int) : int =
  if (a=b) then f(b)
  else f(a)*pi(a+1,b,f)


(*#4 Digits function*)
fun digits(x:int) : int list =
let
  fun dig_help(y:int, zs:int list) : int list =
    if(y div 10 = 0) then y::zs
    else dig_help(y div 10, (y mod 10)::zs)

in
  dig_help(x, [])

end


(*#5 Digital Roots and Addictive Persistence*)
(*additivePersistence*)
fun additivePersistence(x:int) : int =
let
  fun sum_list(xs:int list) : int =
    if (null xs) then 0
    else (hd xs) + sum_list(tl xs)

in
  if(x div 10 = 0) then 0
  else 1 + additivePersistence(sum_list(digits(x)))

end

(*Digital root*)
fun digitalRoot(x:int) : int = 
let
  fun sum_list(xs:int list) : int =
    if (null xs) then 0
    else (hd xs) + sum_list(tl xs)

in
  if(x div 10 = 0) then x
  else digitalRoot(sum_list(digits(x)))

end

