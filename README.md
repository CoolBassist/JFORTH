# JFORTH
A simple one-file interpreter for a custom FORTH language.

```
>>> 3 4 6 + * .
30 :)
>>> : DOUBLE DUP DUP + ;
Function definition for 'DOUBLE' created. :)
>>> 30 DOUBLE .
60 :)
>>> 3 4 > IF 10 . ELSE 14 . THEN
14 :)
```


## Introduction
JFORTH is a language that is an imperative stack-based language that is ran via a shell. It uses a stack for storing numbers for the calculations and expressions the programmer writes, this naturally means the language follows a RPN style.

There are only two types of tokens in the language, symbols, and numbers. Symbols (called "words" in typical FORTH) are your typical functions, they *do stuff*. Numbers are just numbers. When numbers are encountered they are simply pushed onto the stack. When symbols are encountered, the interpreter first checks if the symbol is a built in, such as `CR` (carriage return), `.` (print), `DUP` (duplicate the top of the stack), etc. if it is not a built in, it then checks the user defined functions.

## Examples
```
>>> 3 4 6 + * .
30 :)
```

Explanation: First 3, 4, and 6 are pushed onto the stack. The `+` pops the two most recent numbers on the stack (the 4 and 6), adds them together and pushes `10` back onto the stack. The `*` pops the two most recent numbers on the stack, `10` and `3`, multiplies them and pushes `30` back onto the stack. Then finally `.` simply outputs the top of the stack.

```
>>> : FAC DUP 1 <> IF DUP 1 - FAC * THEN ;
Function definition for 'FAC' created. :)
>>> 5 FAC .
120 :)
```
Recursive factorial function.

## Documentation

### Builtins
Symbol | Explanation
-------|------------
CR | Outputs a new line character.
. | Ouputs the number on top of the stack.
\+ \- \* \/ | Pops two numbers off the stack and pushes the result.
DUP | Takes the number on top of the stack, and pushes it again.
DROP | Discards the number on top of the stack.
\=\= \> \< \>= \<= \<\> | Pops the two numbers on top of the stack, pushes 1 if true, 0 if false.
0\< 0\> | Compares the top of the stack with 0, pushes 1 if true, 0 if false.
SWAP | Swaps the two numbers on top of the stack.
IF | If the number on top of the stack is a non-zero number, proceed as normal. Else skip ahead to an ELSE or THEN token.
ELSE | Skip ahead to a THEN token.
THEN | Do nothing.

A string has the following syntax `."Hello, world!"`. When a string is encountered it will instantly print to the console.

### Writing functions
Function definitions have the following syntax
```
: <FUNCTION NAME> <FUNCTION BODY> ;
```

The function will then entered into a function map, and then if the function name is encountered in a later statement the function body will be executed.
