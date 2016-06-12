// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
//pseudo:

@R2 //init R2 to 0
M=0 

@R1
D=M
@i
M=D //Load R1 into @i. Loop control

(LOOP)
    @i
    D=M
    @END //loop termination
    D;JEQ
    
    @R2 //R2=R0+R2
    D=M
    @R0
    D=D+M
    @R2
    M=D
    
    @i //i--
    M=M-1
    
    @LOOP
    0;JMP
(END)
    @END
    0;JMP