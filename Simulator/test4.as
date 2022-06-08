        lw 0 1 five
        lw 0 2 neg1
        lw 0 3 one
        lw 0 4 point
start   lw 4 6 0 sum of ele1 ~ ele5
        add 7 6 7
        add 4 3 4
        add 1 2 1
        beq 0 1 1
        beq 0 0 start
        halt
five     .fill 5
one     .fill 1
neg1    .fill -1
point   .fill ele1
ele1    .fill 1
ele2    .fill 2
ele3    .fill 3
ele4    .fill 4
ele5    .fill 5
