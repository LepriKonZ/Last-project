;file ps.as
;sample source code
.entry Next
.extern wNumber
STR: .asciz "aBcd"
MAIN: add $3,$5,$9
LOOP: ori $9,-5,$2
	 la val1

	 jmp $4
	 la wNumber
.extern val1
	 .dh 27056
K: .dw 31,-12
END: stop
.entry K
