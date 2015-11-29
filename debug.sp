"scl" "0 2 4 5" gen_vals

2 metro dup 3 pset 0 "scl" tseq 60 + 0.2 1 3 jitter + mtof 0.01 port 0.1 saw 
3000 0.3 moogladder
