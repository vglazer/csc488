A program using loops, including nested loops

Project language:

{
   int i 
   int j
   int total
 
   total = 0
   i = 0
   while i < 10 do
       j = 0
       do
           total = total + 1
	   j = j + 1
       until j = 10
       i = i + 1
   od
   put total, skip
}

Project machine assembly:

            ; back up display for scope's lexic level
            ADDR 0 0

            ; initialise display at LL = 0
            PUSHMT
            SETD 0

            PUSH 0                    ; allocate space for i
            PUSH 0                    ; allocate space for j
            PUSH 0                    ; allocate space for total

            ; total = 0
            ADDR 0 2    
            PUSH 0
            STORE      

            ; i = 0
            ADDR 0 0
            PUSH 0
            STORE

:whilecond  ADDR 0 0 
            LOAD
            PUSH 10 
            LT        ; i < 10 ?
            PUSH [ label exit      ]    
            BF                        ; exit if i >= 10

            ADDR 0 1                  ; j = 0
            PUSH 0
            STORE                     

 :dostart   ADDR 0 2
            LOAD                      ; load total
            PUSH 1    
            ADD       
            ADDR 0 2
            STORE                     ; total = total + 1

            ADDR 0 1                  ; load j
            LOAD                      
            PUSH 1
            ADD
            ADDR 0 1
            STORE                     ; j = j + 1

            ADDR 0 1                  ; load j
            LOAD                      
            PUSH 10
            EQ                        ; j = 10 ?
            PUSH [ label dostart   ]  
            BF

            ADDR 0 0                  ; i = 0
            LOAD                     
            PUSH 1
            ADD
            ADDR 0 0
            STORE                     ; i = i + 1

            PUSH [ label whilecond ] 
            BR

:exit       ADDR 0 2
            LOAD                      ; load total
            PRINTI                    ; put total
            PUSH 012                  ; ASCII for '\n'
            PRINTC                    ; put skip

            ; clean up scope  
            PUSHMT 
            ADDR 0 0 
            SUB
            POPN
