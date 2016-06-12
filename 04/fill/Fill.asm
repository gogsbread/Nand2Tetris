// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.
(LOOP)
    @KBD
    D=M
    @SETWHITE
    D,JEQ
    @SETBLACK
    0,JMP
    
(SETWHITE)
    @color
    M=0
    @COLORSCREEN
    0,JMP
(SETBLACK)
    @color
    M=-1
    @COLORSCREEN
    0,JMP
    
(COLORSCREEN)
    @i
    M=0
    @256
    D=A
    @i
    M=M+D //i=256

    @colptr
    M=0
    @SCREEN
    D=A
    @colptr
    M=M+D //colptr=address of screen buffer

    (COLCOLOR)
        @i
        D=M
        @LOOP //Change
        D;JEQ
        
        @i //i--
        M=M-1

        @j //Paint row
        M=0
        @32
        D=A
        @j
        M=M+D //j=32
        (ROWCOLOR)
            @j
            D=M
            @COLCOLOR
            D;JEQ
            
            @color//get color
            D=M
            @colptr //col pointer
            A=M
            M=D
            @colptr//inc col pointer
            M=M+1
            
            @j //j--
            M=M-1

            @ROWCOLOR
            0;JMP