#include <string.h>
#include "symbols.h"

symbol_t symbols[MAX_SYMBOLS];
int symbol_count = 0;
unsigned short program_counter = 0;

// Find symbol in symbol table
int find_symbol(char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Add symbol to symbol table
void add_symbol(char *name, unsigned short address) {
    if (symbol_count < MAX_SYMBOLS) {
        strcpy(symbols[symbol_count].name, name);
        symbols[symbol_count].address = address;
        symbols[symbol_count].defined = 1;
        symbol_count++;
    }
}
