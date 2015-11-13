#"vals" "60 62 64 67" gen_vals
#
##3 metro 
#90 "2(++)+-" prop
#0.9 1 maygate dup 
#0.01 0.1 0.3 tenv 
#swap 1 "vals" tseq mtof 0.01 port
#
#0.5 1 1 1 fm *
#
#dup 
#
#0.8 1.4 delay 0.1 1 sine 0.01 0.3 scale * +
#
#dup dup 0.94 10000 revsc drop 0.1 * swap 0.5 * +

"wt" 4096 "0 1 4096 -1" gen_line

"seq" "67 63 60 55" gen_vals

"amps" "1 0.1 0.1 0.1" gen_vals

4 120 * 60 / metro 0 pset

0 p 0.01 0.0 0.08 tenv 

0 p dup 16 0 count dup 1 pset 0 eq * 4 0 count dup dup

"seq" tget 12 2 * - 0.1 + mtof 0.1 0 "wt" osc 
swap "seq" tget 12 2 * - 0.1 - mtof 0.1 0 "wt" osc  +
swap "seq" tget 12 3 * - mtof 0.1 0 "wt" osc  +

0.1 1 sine 500 3000 scale 0.1 moogladder * 2 * 
