# Architecture IdeaGuying

## Architecture
- Main
  - Reads file input, calls parse/lex/codegen, writes result to output file
  - Open file -> lex -> parse -> codegen
- Lexer
  - Lex (tokenize) provided line
- Parser
  - Takes lexed line and gathers info (command type, segment, iValue, etc.)
  - Assuming this will be passing enums around, that allow us to navigate with (ie. enum class CommandType { MemoryAccess, Arithmetic}, enum class Arithmetic { add, and, sub, ...})
  - Push enums that describe the lexed instruction into a vector, return vector. This allows us to expand our parsing capabilities by just pushing more/more specific enums into our vector of enums. Codegen will know what code to produce based on what enums it receives
- Codegen
  - Uses information from parser to generate the assembly code
  - Kind of turns into being a bunch of if else/switch/enums to just build up the correct instructions
  - Takes the vector of enums that parser creates, and then generate code based on the enums in that vector.

## Memory Segments Should Generate
- stack pointer
  - @SP
- local
  - @LCL
- this
  - @that
- that
  - @this
- static
  - @filename.i where i is the nth variable
- constant 
  - @X where x is the value that follows constant

***

### Arithmetic

***Generic Assembly for Logical Operations***

- Pop first value off stack pointer and store in D
    - @SP
    - M=M-1
    - @SP
    - A=M
    - D=M
- *if operation only requires one element off stack (neg, eq, and not)*
  - Operate on D (ie. D=-D) 
- *if operation requires two elements off stack (add, sub, gt, lt, and, or, and not)*
    - Store D in R13
        - @R13
        - M=D
    - Pop second value off stack and store in D
        - @SP
        - M=M-1
        - @SP
        - A=M
        - D=M
    - Perform operation
      - @R13
      - operation using D and M (m is first value, D is second value)
- Push D back onto the stack
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1 

***

## Memory Access
- **push segment i**
  - @segment or (@i if segment == constant)
  - if not constant segment
    - A=M
    - D=M
  - else
    - D=A
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **pop segment i**
  - @segment
  - A=M
  - D=M
  - @segment
  - M=M+D
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @segment
  - A=M
  - M=D

pop this 7

@i
D=A
@segment
M=M+D
@SP
M=M-1
@SP
A=M
D=M
@segment
M=D
@i
D=A
@segment
M=M-D

***

## Specific Logical Operations 
- **add**
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @R13
  - M=D
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @R13
  - D=D+M
  - @SP
  - A=M
  - M=D
- **sub**
  - @SP
  - A=M
  - D=M
  - @R13
  - M=D
  - @SP
  - M=M-1
  - A=M
  - D=M
  - @R13
  - D=D-M
  - @SP
  - A=M
  - M=D
- **neg**
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - D=-D
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **eq**
  - (true)
    - D=1
  - (false)
    - D=0
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @true
  - D; JEQ
  - @false
  - D; JNE
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **gt**
  - (true)
    - D=1
  - (false)
    - D=0
  - @SP
  - A=M
  - D=M
  - @R13
  - M=D
  - @SP
  - A=M
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @SP
  - M=M-1
  - @R13
  - D=D-M
  - @true
  - D;JGT
  - @false
  - D;JLT
  - @false
  - D;JEQ
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **lt**
  - (true)
    - D=1
  - (false)
    - D=0
  - @SP
  - A=M
  - D=M
  - @R13
  - M=D
  - @SP
  - A=M
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @SP
  - M=M-1
  - @R13
  - D=D-M
  - @true
  - D;JLT
  - @false
  - D;JGT
  - @false
  - D;JEQ
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **and**
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @R13
  - M=D
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @R13
  - D=M&D
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **or**
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @R13
  - M=D
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - @R13
  - D=M|D
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1
- **not**
  - @SP
  - M=M-1
  - @SP
  - A=M
  - D=M
  - D=!D
  - @SP
  - A=M
  - M=D
  - @SP
  - M=M+1

pop local 4

@4
D=A
@LCL
D=D+A
@R13
M=D
@SP
M=M-1
@SP
A=M
D=M
@R13
A=M
M=D