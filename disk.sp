"bach.wav" diskin
#"fold4_wrap5.flac" diskin
#"samples/aria.wav" diskin

# add delay
dup 0 p 0 0.9 scale 0.4 delay 

1 p 0 1 scale * +

# add reverb
#dup dup 0 p 0.9 0.99 scale 10000 revsc drop 0.1 * swap 0.8 * + 

1 p 0 1 scale 2 pset
