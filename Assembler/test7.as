neg1    nor 0 0 1
add 1 1 2
nor 0 2 3 reg3 is 1
five   lw 3 4 7 reg4 is 5
start   beq 4 6 done loop until reg6 == 5
add 6 3 6
beq 0 0 start
done    halt
five    .fill 5 (error)
