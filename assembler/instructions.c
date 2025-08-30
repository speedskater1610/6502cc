#include "instructions.h"

// Complete 6502 instruction set with all addressing modes
instruction_t instructions[] = {
    // ADC - add with carry
    {"ADC", 0x69, IMM}, 
    {"ADC", 0x65, ZPG}, 
    {"ADC", 0x75, ZPX},
    {"ADC", 0x6D, ABS}, 
    {"ADC", 0x7D, ABX}, 
    {"ADC", 0x79, ABY},
    {"ADC", 0x61, IZX}, 
    {"ADC", 0x71, IZY},
    
    // AND - logic AND
    {"AND", 0x29, IMM}, 
    {"AND", 0x25, ZPG}, 
    {"AND", 0x35, ZPX},
    {"AND", 0x2D, ABS}, 
    {"AND", 0x3D, ABX}, 
    {"AND", 0x39, ABY},
    {"AND", 0x21, IZX}, 
    {"AND", 0x31, IZY},
    
    // ASL - arithmetic shift left
    {"ASL", 0x0A, IMP}, 
    {"ASL", 0x06, ZPG}, 
    {"ASL", 0x16, ZPX},
    {"ASL", 0x0E, ABS}, 
    {"ASL", 0x1E, ABX},
    
    // BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS - branches
    {"BCC", 0x90, REL}, 
    {"BCS", 0xB0, REL}, 
    {"BEQ", 0xF0, REL},
    {"BMI", 0x30, REL}, 
    {"BNE", 0xD0, REL}, 
    {"BPL", 0x10, REL},
    {"BVC", 0x50, REL}, 
    {"BVS", 0x70, REL},
    
    // BIT - bit test
    {"BIT", 0x24, ZPG}, 
    {"BIT", 0x2C, ABS},
    
    // BRK - break
    {"BRK", 0x00, IMP},
    
    // CLC, CLD, CLI, CLV - clear flags
    {"CLC", 0x18, IMP}, 
    {"CLD", 0xD8, IMP}, 
    {"CLI", 0x58, IMP}, 
    {"CLV", 0xB8, IMP},
    
    // CMP - compare Accumulator
    {"CMP", 0xC9, IMM}, 
    {"CMP", 0xC5, ZPG}, 
    {"CMP", 0xD5, ZPX},
    {"CMP", 0xCD, ABS}, 
    {"CMP", 0xDD, ABX}, 
    {"CMP", 0xD9, ABY},
    {"CMP", 0xC1, IZX}, 
    {"CMP", 0xD1, IZY},
    
    // CPX - compare X reg
    {"CPX", 0xE0, IMM}, 
    {"CPX", 0xE4, ZPG}, 
    {"CPX", 0xEC, ABS},
    
    // CPY - Compare Y reg
    {"CPY", 0xC0, IMM}, 
    {"CPY", 0xC4, ZPG}, 
    {"CPY", 0xCC, ABS},
    
    // DEC - dec Memory
    {"DEC", 0xC6, ZPG}, 
    {"DEC", 0xD6, ZPX}, 
    {"DEC", 0xCE, ABS}, 
    {"DEC", 0xDE, ABX},
    
    // DEX, DEY - dec X/Y
    {"DEX", 0xCA, IMP}, 
    {"DEY", 0x88, IMP},
    
    // EOR - Exclusive OR
    {"EOR", 0x49, IMM}, 
    {"EOR", 0x45, ZPG}, 
    {"EOR", 0x55, ZPX},
    {"EOR", 0x4D, ABS}, 
    {"EOR", 0x5D, ABX},
    {"EOR", 0x59, ABY},
    {"EOR", 0x41, IZX},
    {"EOR", 0x51, IZY},
    
    // XOR -  XOR       *note this is the same as EOR instruction but makes it easier on my part to recognize
    {"EOR", 0x49, IMM}, 
    {"EOR", 0x45, ZPG}, 
    {"EOR", 0x55, ZPX},
    {"EOR", 0x4D, ABS}, 
    {"EOR", 0x5D, ABX},
    {"EOR", 0x59, ABY},
    {"EOR", 0x41, IZX},
    {"EOR", 0x51, IZY},

    // INC - inc Memory
    {"INC", 0xE6, ZPG}, 
    {"INC", 0xF6, ZPX}, 
    {"INC", 0xEE, ABS}, 
    {"INC", 0xFE, ABX},
    
    // INX, INY - inc X/Y
    {"INX", 0xE8, IMP}, 
    {"INY", 0xC8, IMP},
    
    // JMP - Jump
    {"JMP", 0x4C, ABS}, 
    {"JMP", 0x6C, IND},
    
    // JSR - Jump to Subroutine
    {"JSR", 0x20, ABS},
    
    // LDA - Load Accumulator
    {"LDA", 0xA9, IMM}, 
    {"LDA", 0xA5, ZPG}, 
    {"LDA", 0xB5, ZPX},
    {"LDA", 0xAD, ABS}, 
    {"LDA", 0xBD, ABX}, 
    {"LDA", 0xB9, ABY},
    {"LDA", 0xA1, IZX}, 
    {"LDA", 0xB1, IZY},
    
    // LDX - Load X Register
    {"LDX", 0xA2, IMM}, 
    {"LDX", 0xA6, ZPG}, 
    {"LDX", 0xB6, ZPY},
    {"LDX", 0xAE, ABS}, 
    {"LDX", 0xBE, ABY},
    
    // LDY - Load Y Register
    {"LDY", 0xA0, IMM}, 
    {"LDY", 0xA4, ZPG}, 
    {"LDY", 0xB4, ZPX},
    {"LDY", 0xAC, ABS}, 
    {"LDY", 0xBC, ABX},
    
    // LSR - logical Shift Right
    {"LSR", 0x4A, IMP}, 
    {"LSR", 0x46, ZPG}, 
    {"LSR", 0x56, ZPX},
    {"LSR", 0x4E, ABS}, 
    {"LSR", 0x5E, ABX},
    
    // NOP - No Operation
    {"NOP", 0xEA, IMP},
    
    // ORA - Logical OR
    {"ORA", 0x09, IMM}, 
    {"ORA", 0x05, ZPG}, 
    {"ORA", 0x15, ZPX},
    {"ORA", 0x0D, ABS}, 
    {"ORA", 0x1D, ABX}, 
    {"ORA", 0x19, ABY},
    {"ORA", 0x01, IZX}, 
    {"ORA", 0x11, IZY},
    
    // PHA, PHP, PLA, PLP - Stack operations
    {"PHA", 0x48, IMP}, 
    {"PHP", 0x08, IMP}, 
    {"PLA", 0x68, IMP}, 
    {"PLP", 0x28, IMP},
    
    // ROL - Rotate Left
    {"ROL", 0x2A, IMP}, 
    {"ROL", 0x26, ZPG}, 
    {"ROL", 0x36, ZPX},
    {"ROL", 0x2E, ABS}, 
    {"ROL", 0x3E, ABX},
    
    // ROR - Rotate Right
    {"ROR", 0x6A, IMP}, 
    {"ROR", 0x66, ZPG}, 
    {"ROR", 0x76, ZPX},
    {"ROR", 0x6E, ABS}, 
    {"ROR", 0x7E, ABX},
    
    // RTI, RTS - Return
    {"RTI", 0x40, IMP},
    {"RTS", 0x60, IMP},
    
    // SBC - Subtract with Carry
    {"SBC", 0xE9, IMM}, 
    {"SBC", 0xE5, ZPG}, 
    {"SBC", 0xF5, ZPX},
    {"SBC", 0xED, ABS}, 
    {"SBC", 0xFD, ABX}, 
    {"SBC", 0xF9, ABY},
    {"SBC", 0xE1, IZX}, 
    {"SBC", 0xF1, IZY},
    
    // SEC, SED, SEI - Set flags
    {"SEC", 0x38, IMP}, 
    {"SED", 0xF8, IMP}, 
    {"SEI", 0x78, IMP},
    
    // STA - Store Accumulator
    {"STA", 0x85, ZPG}, 
    {"STA", 0x95, ZPX}, 
    {"STA", 0x8D, ABS},
    {"STA", 0x9D, ABX}, 
    {"STA", 0x99, ABY}, 
    {"STA", 0x81, IZX}, 
    {"STA", 0x91, IZY},
    
    // STX - Store X Register
    {"STX", 0x86, ZPG}, 
    {"STX", 0x96, ZPY}, 
    {"STX", 0x8E, ABS},
    
    // STY - Store Y Register
    {"STY", 0x84, ZPG}, 
    {"STY", 0x94, ZPX}, 
    {"STY", 0x8C, ABS},
    
    // TAX, TAY, TSX, TXA, TXS, TYA - Transfer
    {"TAX", 0xAA, IMP}, 
    {"TAY", 0xA8, IMP}, 
    {"TSX", 0xBA, IMP},
    {"TXA", 0x8A, IMP}, 
    {"TXS", 0x9A, IMP}, 
    {"TYA", 0x98, IMP}
};

const int NUM_INSTRUCTIONS = sizeof(instructions) / sizeof(instruction_t);
