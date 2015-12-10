# 0.5 1 'aur' tset
# 0.5 2 'aur' tset

# X Y control

# 4 'aur' tget
# randomize X
0 1 1 randi
1 'aur' tset

# 5 'aur' tget
# randomize Y
0 1 1 randi
2 'aur' tset


0 1 2 randi 8 'aur' tset
0 1 2 randi 9 'aur' tset
0 1 1 randi 10 'aur' tset

1 2 'aur' tget - dup dup
11 'aur' tset
12 'aur' tset
13 'aur' tset

# sets jitter amount
0.1 1 'aur' tget * 0 pset
1 'aur' tget 1 * 1 pset
1 'aur' tget 32 * 2 pset

'tbl' '10 62' gen_vals
'seq' '60 63 74 70 60 63 70 68 79' gen_vals

50 4 / '++++--2(++)+-' prop 
# 14 'aur' tget dup 15 'aur' tset
delay1
dup 0 'seq' tseq 1 'tbl' tset
4 1 'tbl' 'poly' tpoly 

0 0 'poly' polyget 
2 0 1 'poly' polyget 
4 - 2 tenv 
0 2 'poly' polyget 
0 p 
1 p  
2 p 
jitter + mtof 0.3 saw * 

1 0 'poly' polyget 
2 1 1 'poly' polyget 
4 - 2 tenv 
1 2 'poly' polyget 
0 p 
1 p  
2 p 
jitter + mtof 0.3 saw * 

+

2 0 'poly' polyget 
2 2 1 'poly' polyget 
4 - 2 tenv 
2 2 'poly' polyget 
0 p 
1 p  
2 p 
jitter + mtof 0.3 saw * 

+

3 0 'poly' polyget 
2 3 1 'poly' polyget 
4 - 2 tenv 
3 2 'poly' polyget 
0 p 
1 p  
2 p 
jitter + mtof 0.3 saw *  

+

0 'aur' tget  +

1000 butlp  
500 buthp 
dup 0.8 2 delay 0.3 * 

1000 buthp 
swap 0.2 1 sine 0 1 scale * + 

14 'aur' tget 1 0 3 tenv 
1 'tbl' tget mtof 0.01 2 7 2 fm * +

0 'aur' tget 500 buthp dup 
2 'aur' tget 0.9 *
3 delay + +
dup 

dup 0.97 8000 revsc drop 
1 'aur' tget 0.1 * 0.3 +
* + 

0.8 1 2 'aur' tget - * 0.1 + * 

16 'aur' tget  +

# 17 'aur' tget 0 f
18 'aur' tget 1 f
