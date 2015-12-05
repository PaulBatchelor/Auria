# P field guide
# p 0 returns x cordinate (-1, 1 range)
# p 1 returns y cordinate (-1, 1 range)
# p 2 sets the background shade (0-1 range, 0 = black, 1 = white)
# p 3 will create a tick on a line when sent a trigger
# p 4 returns a tick created by the left button trigger

'tbl' '10 62' gen_vals
'stk' '0 0' gen_vals
'seq' '60 63 74 70 60 63 70 68 79' gen_vals

50 4 / '++++--2(++)+-' prop dup 0 'stk' tset dup 0 'seq' tseq 1 'tbl' tset
4 1 'tbl' 'poly' tpoly 

0 0 'poly' polyget 2 0 1 'poly' polyget 4 - 2 tenv 0 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw * 1 0 'poly' polyget 2 1 1 'poly' polyget 4 - 2 tenv 1 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw * 2 0 'poly' polyget 2 2 1 'poly' polyget 4 - 2 tenv 2 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw * 3 0 'poly' polyget 2 3 1 'poly' polyget 4 - 2 tenv 3 2 'poly' polyget 
0.1 2 3 jitter + mtof 0.3 saw *  + + +  

1 p -1 * 1000 4000 scale butlp 300 buthp

0.4 *

1 p -1 * 0.4 1 scale *

4 p 0.01 0 0.8 tenv 4 p 1 'seq' tseq mtof 0.4 1 7 4 p 0.001 0 0.01 tenv 3 * fm *  +

dup 0.9 2 delay 0.3 * 100 buthp 
1 p -1 * 0.1 1 scale * + 

# 5 p 0.5 * +

dup dup 0.97 8000 revsc drop 0.4 * + 

0 'stk' tget 4 p + 3 pset

4 p 0.01 0 0.8 tenv 0.3 * 
# 0 'stk' tget 0.5 0 2 tenv + 2 pset
1 p -1 * 0 1 scale + 2 pset
