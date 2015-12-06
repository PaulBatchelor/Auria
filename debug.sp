'sine' 4096 gen_sine

#0 1 1 randi 6 pset
#0 1 1 randi 7 pset

0.1 1 0.25 'sine' osc 0 1 scale 6 pset
0.1 1 0 'sine' osc -5 1 scale 8 pset

#9 p 0.1 1 3 jitter 1 + * 6 pset
10 p 7 pset


0
