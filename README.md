# Interpreter-Parser-Translator-for-2048
Interpreter[Parser Translator] for 2048 using C, Flex, Bison

ROHIT GARG
2018A7PS0193G

#INTERPRETER FOR 2048 GAME USING C, FLEX, BISON#

## The Game architecture consists of 3 sections:
A. 2048 game implementation and interface file [driver.c and driver.h file respectively]
B. Scanner [scanner.l]
C. Parser [parser.y]

#Note: The game implementation makes use of a struct State which consists of 4X4 board. Each Tile on board has been made as a C-style struct consisting of 1) a value 2) number of variable names assigned till now 3) array of variable names.


## The Contents of zip submission file are as follows:
* driver.h
* driver.c
* scanner.l
* parser.y 
* Makefile 
* README.txt


#For Compilation:

1) In terminal execute:
make

2) To use, run the program in terminal:
./out 

3) To exit from the program, enter: 
exit

## Note: One can always exit the program using Ctrl + C.

## To clean temporary/intermediate generated file:-
make clean

## Assumption
 
Given a row ` 4 2 2 4 ` than, SUBTRACT LEFT will result in ` 4 4 0 0 `.


