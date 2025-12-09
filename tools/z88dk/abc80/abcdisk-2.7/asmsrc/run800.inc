;;
;; BASIC byte prefix to invoke the loader.  On return try to execute
;; a CHAIN, which will fail if the filename is invalid; at that point
;; ON ERROR GOTO will point to END.  If desired the ON ERROR GOTO
;; pointer at FF42h can be pointed to a different chunk of BASIC
;; bytecode instead; to override the CHAIN string even if valid
;; use RST 10h to trigger an error instead of returning.
;;
;; The filename string (if valid) should be padded with CR.
;;
_basic:
	defb 133		; ON ERROR GOTO
b1:	defw b_end-b1-1
	defb 221 + 11		; PUSH int 11
	defb 209		; Function call, 1 parameter
	defb 25			; SYS()		SYS(11) = BOFA
	defb 199		; PUSH int
	defw _start
	defb 156		; + int
	defb 209		; Function call, 1 parameter
	defb 23			; CALL()
	defb 184		; End int expression
	defb 134, 11		; CHAIN
	defb 203, 16		; String, 16 bytes
b_chainstr:
	defm ":CHAINFILE:HERE:"	; This is an invalid filename
	defb 185		; End string expression (executes CHAIN)
b_end:
	defb 146		; END
