'tbl' '10 62' gen_vals
'stk' '0 0' gen_vals
'seq' '60 63 74 70 60 63 70 68 79' gen_vals

50 4 / '++++--2(++)+-' prop dup 0 'stk' tset dup 0 'seq' tseq 1 'tbl' tset
4 1 'tbl' 'poly' tpoly 

0 0 'poly' polyget 2 0 1 'poly' polyget 4 - 2 tenv 0 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw * 1 0 'poly' polyget 2 1 1 'poly' polyget 4 - 2 tenv 1 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw * 2 0 'poly' polyget 2 2 1 'poly' polyget 4 - 2 tenv 2 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw * 3 0 'poly' polyget 2 3 1 'poly' polyget 4 - 2 tenv 3 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw *  + + +  1000 butlp  500 buthp 

0.4 *

4 p 0.01 0 0.8 tenv 4 p 1 'seq' tseq mtof 0.4 1 7 4 p 0.001 0 0.01 tenv 3 * fm *  +

dup 0.9 2 delay 0.3 * 800 buthp swap 0.2 1 sine 0.2 1 scale * + 


dup dup 0.97 8000 revsc drop 0.4 * + 

0 'stk' tget 4 p + 3 pset

4 p 0.01 0 0.8 tenv 0.3 * 
0 'stk' tget 0.5 0 2 tenv + 2 pset


