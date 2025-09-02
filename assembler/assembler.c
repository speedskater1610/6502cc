#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "types.h"
#include "symbols.h"
#include "utils.h"
#include "instructions.h"

#define MAX_OUTPUT 4096
#define MAX_PASSES 2

char* assemble_6502(char* assembly_code) {
    static char hex_output[MAX_OUTPUT];
    unsigned char binary[MAX_OUTPUT/3];
    int output_pos = 0;

    // reset globals
    symbol_count = 0;
    program_counter = 0;
    memset(hex_output, 0, sizeof(hex_output));
    memset(binary, 0, sizeof(binary));

    // ... rest of the two-pass logic here ...
}
