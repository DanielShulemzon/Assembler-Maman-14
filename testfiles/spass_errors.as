; Second pass errors

DATALABEL: .string "My String.$#"
MYSTRING: .string "My string"
MYDATA: .data 5,4,3,2,1
X: .string "XXXXX"

.extern EXTERNLABEL
.entry DATALABEL
.entry CODELABEL
CODELABEL: mov r0, r1

; external label cannot be defined as an entry one as well
.entry EXTERNLABEL

;will raise a warning.
LABEL: .entry SOMETHING 

.entry
.entry SOMEUNKNOWNLABEL
.entry MYSTRING
.entry MYDATA
.entry MYDATA0
.entry 0MYDATA
.entry 0MYSTRING
.entry 0
.entry NULL

;code lines errors:
mov #4, NOTFOUNDLABEL