4 'aur' tget
1 'aur' tset 

5 'aur' tget
2 'aur' tset 

7 'aur' tget -5 *
3 'aur' tset 

'space' 'space.wav' loadfile

1 5 'aur' tget - 1.9 * 0.1 + 
'space' tbldur / 
0 phasor 

1 0 0 'space' tabread

dup 

4 'aur' tget 0.9 *
7 'aur' tget 0.5 * 0.01 +
1 
vdelay +
