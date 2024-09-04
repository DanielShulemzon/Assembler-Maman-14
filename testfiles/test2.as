macr test_macro
 mov r1, r2
 add r3, r4
 endmacr

macr another_macro
 lea STR, r5
 sub r6, r7
 endmacr

MAIN: mov r1, K
 prn #25
 test_macro
 bne LOOP
 lea LIST, r2
 jmp END
 dec EXTERNALLABEL
 inc r1
 another_macro
 stop

LOOP: add r2, r3
 sub r4, r5
 prn #56
 jmp MAIN

END: stop

.extern EXTERNALLABEL
.entry MAIN
.entry END

STR: .string "example"
LIST: .data 10, -20
.data 50
K: .data 99