
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

```
PUSH   Push value (number/string) to stack
COP    Print constant value from code    
JEQ0   Jump to label if stack top == 0    
JGT0   Jump to label if stack top > 0    
HX     Exit program immediately    
ADD    Add top two stack numbers    
PRINT  Print top stack value    
NL     Print newline    
CALL   Call subroutine (pushes return addr)    
HLT    Halt program    
DP     Duplicate top stack value    
SUB    Subtract (top from second)    
CIP    Get user input (→stack)    
RET    Return from subroutine
SWP    Swap top two stack
POP    Remove top stack value
JMP    Jump to label (no return)    
MUL    Multiply top two numbers    
DIV    Divide (second / top)    
MOD    Modulo (second % top)
RND    Random num (min/max → rand)
```

## Contributing

Feel free to fork, submit issues, or pull requests to improve SeptionLang.
