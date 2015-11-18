#"bach.wav" diskin
"samples/aria.wav" diskin

# add delay
dup 0.5 0.4 delay 0.5 * +

# add reverb
dup dup 0.97 10000 revsc drop 0.1 * swap 0.8 * + 

