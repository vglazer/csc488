A program using recursive functions with parameters

Project language:

{
    int factorial(int n) :
    {
        if (n = 0) then
              result 1
        else
              result factorial(n - 1) * n
        fi
    }
    put factorial(5)
}


Assembly code: 

; start of body of main scope
:startpt    ADDR 0 0                 ; back up display 
            PUSH 0                   ; add space for return of factorial(5)
            ADDR 1 0 
            PUSH [ label retaddr ] 
            PUSH 5                   ; push the arg
            PUSH [ label factorial ] 
            BR
retaddr2:   SETD 1 
            PRINTI
            PUSHMT                   ; pop locals
            ADDR 0 0 
            SUB
            POPN
            SETD 0                   ; restore display

; declaration for factorial
factorial:  PUSHMT                   ; beginning of factorial
            PUSH 1                   ; push number of args 
            SUB
            SETD 1                   ; set the lexic level

                                     ; if condition expression
	    PUSH 0               
	    ADDR 1 0
	    LOAD
	    EQ
	    PUSH [ label condf ] 
	    BF 

	    ADDR 1 0                ; result 1
	    PUSH 3
	    SUB
	    PUSH 1                  ; push value of expn
	    STORE
	    PUSH [ label funclean ]  
	    BR 

:condf      ADDR 1 0                ; address of return value
            PUSH 3
            SUB 

                                    ; call factorial
            PUSH 0                  ; space for result
            ADDR 1 0                ; back up display 
            PUSH [ label retaddr ]  ; push return address 

                                    ; load arguments 
            ADDR 1 0                ; load the first arg passed in, (n)
            LOAD                    ; load n
            PUSH 1   
            SUB                     ; subtract 1 from n

            PUSH [ label factorial ]; go to beginning of function
            BR

:retaddr    SETD 1 
                                    ; multiply result by n
            ADDR 1 0 
            LOAD                    ; load n again
            MUL                     ; multiply n with factorial(n-1) 

            STORE                   ; store result  
            PUSH [ label funclean ] 
            BR                      ; end result factorial(n-1) * n

:funclean   PUSHMT 
            ADDR 1 0
            SUB
            POPN
            BR

