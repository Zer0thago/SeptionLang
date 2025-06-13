/*
# SeptionLang

SeptionLang is a simple stack-based interpreted language with a minimalistic instruction set, designed for learning and experimenting with virtual machines and interpreters.

## Features

- Stack operations: push, pop, duplicate, swap  
- Arithmetic: add, subtract, multiply, divide, modulo, random number generation  
- Control flow: conditional and unconditional jumps, function calls, returns  
- I/O operations: print strings and values, console input  
- Labels and subroutines support  
- Simple error handling with meaningful messages

## Usage

create a main.sl file.

## Hello world
```
COP "Hello World"
HX 
```

## Math
```
PUSH 5
PUSH 8
ADD ; you can also use ADD / MUL / MOD / DIV
PRINT
HX
```

## Functions
```
COP "Hello "
CALL world
HX 

world:
COP "WORLD"
HX 
```

## Random Number Generator Based on Min / Max
```
COP "Enter Min: "
CIP
COP "Enter Max: "
CIP
CALL generate
HX       ; exit

generate:
RND      ; generate
PRINT    ; print
NL       ; newline
HLT  
```



## Opcodes

Supported opcodes include (but are not limited to):

- PUSH, POP, DP, SWP  
- ADD, SUB, MUL, DIV, MOD, RND  
- JEQ0, JGT0, JMP, CALL, RET, HLT  
- COP, PRINT, NL, CIP, HX  

## Contributing

Feel free to fork, submit issues, or pull requests to improve SeptionLang.
*/
