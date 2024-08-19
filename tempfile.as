; file ps.am
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
macr m1 m2
 inc r6
 mov r3, W
 1
 2
 3
 4
 5
 6
 7
 8
 9
 0
endmacr
lea STR, r6
inc r6
mov r3, W
sub r1, r4
bne END
cmp val1, #-6
bne END[r15]
dec K
.entry MAIN
m1
sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, -9
m1
.data -100
.entry K
K: .data 31
.extern val1