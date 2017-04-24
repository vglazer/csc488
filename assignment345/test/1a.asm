A short program that doesn't use arrays, functions or 
procedures:

Project language:

{
    int i
    int j

    i = 1
    j = 1

    if 3 + i * 5 / 2 = j then
        put "hello world!", skip
    else 
        put "42", skip
    fi
}

Project machine assembly:

       ; back up display for scope's lexic level
       ADDR 0 0

       ; set up for scope (somewhat useless in this case)  
       PUSHMT       ; update display
       SETD 0       
       
       PUSH 0   ; allocate space for i
       PUSH 0   ; allocate space for j

       ADDR 0 0 ; LL(i) = ON(i) = 0
       PUSH 1   
       STORE    ; i = 1
       ADDR 0 1 ; LL(j) = 0, ON(j) = 1
       PUSH 1
       STORE    ; j = 1

       ADDR 0 0  
       LOAD     ; load i 
       PUSH 5
       MUL      ; i *= 5
       PUSH 2
       DIV      ; i /= 2
       PUSH 3
       ADD      ; i += 3
       ADDR 0 1
       LOAD     ; push j
       EQ       ; 3 + i * 5 / 2 = j ?
       PUSH [ label else ]  
       BF       ; jump to else branch if i != j

       PUSH 104 ; push 'h'
       PRINTC   ; print 'h'
       PUSH 101 ; push 'e'
       PRINTC   ; print 'e'
       PUSH 108 ; push 'l'
       PRINTC   ; print 'l'
       PUSH 108 ; push 'l' 
       PRINTC   ; print 'l'
       PUSH 111 ; push 'o'
       PRINTC   ; print 'o'
       PUSH 032 ; push ' '
       PRINTC   ; print ' ' 
       PUSH 119 ; push 'w'
       PRINTC   ; print 'w'
       PUSH 111 ; push 'o'
       PRINTC   ; print 'o'
       PUSH 114 ; push 'r'
       PRINTC   ; print 'r'
       PUSH 108 ; push 'l'
       PRINTC   ; print 'l'
       PUSH 100 ; push 'd'
       PRINTC   ; print 'd'
       PUSH 033 ; push '!'
       PRINTC   ; print '!'
       PUSH 012 ; push '\n'
       PRINTC   ; print '\n'

:else  PUSH 50  ; push '4'
       PRINTC   ; print '4'
       PUSH 52  ; push '2'
       PRINTC   ; print '2'
       PUSH 012 ; push '\n'
       PRINTC   ; print '\n'

       ; clean up scope  
       PUSHMT 
       ADDR 0 0 
       SUB
       POPN

