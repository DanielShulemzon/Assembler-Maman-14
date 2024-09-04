
macr myMacro
 inc r1
endmacr

;label errors:

LABEL: mov r0,r1
DATALABEL: .data -0,5,6,7,2,6
LABEL: .string "same label twice!"
CODELABEL: mov r0,r1
LOCALCODE: add r0,r1
jsr: mov r1,r2
endmacr: mov r2,r3
myMacro: .string "already a macro name!"
.extern LOCALCODE
.extern
.extern ILL@EGAL_LABEL
.extern SOMELABEL
.extern ToooooooooooooooooooooooLongggggggLabelllllllllll
SOMELABEL: .string "second definition!"
VERY_VERY_VERY_VERY_VERY_LONG_LABEL_DEFINITION_SO_ITS_ILLEGAL: .data 0
*INVALID&CHARS%: .data 5
5StartsWithNum: .data -6

;string and data errors:

.string "No closing
.string No opening"
.string Not at all
.data ,5,6,-7
.data 5,,6,-7
.data 5,6,,-7
.data 5,6,-7,
.data 5 6 -7
.data 5 6 , -7
.data 5 , 6 -7,
.data 0.5
.data 5.9
.data -0.1
.data .5


;code errors:

; Operand count errors:

mov
mov r0
mov r0,LABEL,#5
cmp
cmp LABEL
cmp #-0,MYLABEL,HERE
add
add r7
add #5,#6,#7
sub
sub LABEL
sub #4,LABEL,r0
lea
lea operand
lea firstoperand,secondoperand
clr
clr ope,rand
not
not r4, label
inc
inc r6,r5,r4
dec
dec lambdaX,XequalsY
jmp
jmp word1,word2
bne
bne hello, world
red alpha,beta
red
prn
prn #48,#56
prn #4,#0,#9
jsr
jsr jump , #60
rts hello
rts hello,goodbye
stop myprog
stop myprog, anotherMyProg

; Addressing type errors

mov #6,#6

add r0, #7

sub r1, #10

lea LABEL, #6
lea #6, r0
lea *r0, r1
lea r1, *r2

clr #1

not #2

inc #3

dec #4

jmp #5
jmp r0

bne #6
bne r1

red #7

jsr #8
jsr r2