#!/bin/bash
#-------------------------------------------------------------- 
COMPILER=../src/compiler488

if [ -f $COMPILER ] 
then 
    echo "----------------------------------------------------------------"
    echo "q_a1: a program that doesn't use arrays, functions or procedures"
    echo "Expected Output: C = 0; D = 1; E = 1; F = 0"
    $COMPILER ./q_a1
    echo "----------------------------------------------------------------"
    echo "q_a2: a program that doesn't use arrays, functions or procedures"
    echo "Expected Output: b = 5; c = 8; d = -5; e = 5; f = -20; g = 2"
    echo "                 h = -17; k = 10"
    $COMPILER ./q_a2
    echo "----------------------------------------------------------------"
    echo "q_a3: a program that doesn't use arrays, functions or procedures"
    echo "Expected Output: i < j"
    echo "                 k < l"
    $COMPILER ./q_a3
    echo "----------------------------------------------------------------"
    echo "q_b: a program using arrays but not functions or procedures"
    echo "Expected Output: success"
    $COMPILER ./q_b
    echo "----------------------------------------------------------------"
    echo "q_c: a program using loops including nested loops"
    echo "Expected Output: i = 12"
    $COMPILER ./q_c
    echo "----------------------------------------------------------------"
    echo "q_d: a program using non-recursive procedures without parameters"
    echo "Expected Output: odd: 1"
    echo "                 even: 2"
    echo "                 odd: 3"
    echo "                 even: 4"
    echo "                 odd: 5"
    echo "                 even: 6"
    echo "                 odd: 7"
    $COMPILER ./q_d
    echo "----------------------------------------------------------------"
    echo "q_e: a program using non-recursive functions with parameters"
    echo "Expected Output: trying: a mod b, a > b ... correct"
    echo "                 trying: a mod b, a < b ... correct"
    $COMPILER ./q_e
    echo "----------------------------------------------------------------"
    echo "q_f: a program using recursive functions with parameters"
    echo "Expected Output: trying 0! ... correct"
    echo "                 trying 7! ... correct"
    $COMPILER ./q_f
    echo "----------------------------------------------------------------"
    echo "q_h: a program that uses a procedure declared within another procedure"
    echo "Expected Output: inner proc sees i = 6"
    echo "                 outer proc sees i = 6"
    $COMPILER ./q_h
    echo "----------------------------------------------------------------"
    echo "display_test: a program that tests that the display is correctly"
    echo "              updated throughout execution."
    echo "Expected Output: i = 10 (expected 10)"
    echo "                 i = 10 (expected 10)"
    echo "                 bar returns 14"
    echo "                 i = 10 (expected 10)"
    echo "                 i = 10 (expected 10)"
    $COMPILER ./display_test
else
    echo "Compiler not present. Run make or something."
fi

