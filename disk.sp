"bach.wav" diskin
#"samples/aria.wav" diskin

# add delay
dup 1 p 0 0.9 scale 0.4 delay 0.5 * +

# add reverb
dup dup 0 p 0.9 0.99 scale 10000 revsc drop 0.1 * swap 0.8 * + 

