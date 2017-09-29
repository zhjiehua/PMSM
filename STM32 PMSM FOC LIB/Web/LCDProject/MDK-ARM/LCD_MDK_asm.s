 				 AREA    NoInit,NOINIT
				
__reg_sp            DCD 0
g_ImportedFunctions DCD 0

				 EXPORT g_ImportedFunctions

				 AREA    |.text|, CODE, READONLY

; LCDI_Entry
LCDI_Entry    	 PROC
                 EXPORT  LCDI_Entry             [WEAK]
				 IMPORT  LCDI_SetUIImportedFunctions
				 IMPORT  __scatterload
	 
				 PUSH    {LR}
				 PUSH    {R0-R7}
				 MOV     R1,R8
				 MOV     R2,R9
				 MOV     R3,R10
				 MOV     R4,R11
				 MOV     R5,R12
				 PUSH    {R1-R5}
				 
				 LDR     R1,=__reg_sp ; save original SP value
				 MOV     R2,R13
				 STR     R2,[R1]
				 				 
				 LDR     R1,=g_ImportedFunctions ; save Imported Functions
				 STR     R0,[R1]
                 
				 LDR     R1, =__scatterload
                 BLX     R1
				 
				 POP     {R1-R5}
				 MOV     R8,R1
				 MOV     R9,R2
				 MOV     R10,R3
				 MOV     R11,R4
				 MOV     R12,R5
				 POP     {R0-R7}
				 POP     {PC}
				 
                 ENDP
				 
; main_x
main_x		 	 PROC
			     EXPORT  main_x             	[WEAK]
				 IMPORT  exportedFunctions
				 
				 LDR     R0,=__reg_sp ; restore original SP value
				 LDR     R1,[R0]
				 MOV     R13,R1
				 
				 POP     {R1-R5}
				 MOV     R8,R1
				 MOV     R9,R2
				 MOV     R10,R3
				 MOV     R11,R4
				 MOV     R12,R5
				 POP     {R0-R7}
				 LDR	 R0,=exportedFunctions
				 POP     {PC}
				 
				 ENDP
				 
				 END