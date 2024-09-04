; source file to show pre-assembler errors - illegal macro definitons.

; normal macro definiton.
macr my_macr
 mov *r0, r1
 LIST: .data 6, -9
endmacr

; illegal macro and endmacr definitons:
macr my_macr
 K: .string "abcd"
 inc *r1
endmacr hello

; reserved word
macr jsr
 cmp #31, #32
endmacr

; no name provided
macr
 mov r6, *r2;
endmacr

; too many arguements for macr;
macr word1 word2
 .data 1,2,3,4,5
 .data 6,7,8
endmacr
 
my_macr
.string "Hello"

word1
