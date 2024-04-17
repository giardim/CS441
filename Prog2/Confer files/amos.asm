; junky program that prints my name
; ...hardly tests anything :-/

!main
    ; alpha "Amos"
    0041
    006d
    006f
    0073

    6041    draw 'A'
    8003    right 3
    6073    draw 's'
    9ffe    left 2
    606d    draw 'm'
    8001    right
    606f    draw 'o'
    8002    right 2
    b000    halt
