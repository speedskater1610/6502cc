#ifndef TYPES_H
#define TYPES_H

// Addressing modes
typedef enum {
    IMP,    // Implied
    IMM,    // Immediate #$nn
    ZPG,    // Zero Page $nn
    ZPX,    // Zero Page,X $nn,X
    ZPY,    // Zero Page,Y $nn,Y
    ABS,    // Absolute $nnnn
    ABX,    // Absolute,X $nnnn,X
    ABY,    // Absolute,Y $nnnn,Y
    IND,    // Indirect ($nnnn)
    IZX,    // Indexed Indirect ($nn,X)
    IZY,    // Indirect Indexed ($nn),Y
    REL     // Relative (branches)
} addr_mode_t;

// Instruction definition
typedef struct {
    char mnemonic[4];
    unsigned char opcode;
    addr_mode_t mode;
} instruction_t;

// Symbol table entry
typedef struct {
    char name[32];
    unsigned short address;
    int defined;
} symbol_t;


#endif
