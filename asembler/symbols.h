#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "types.h"

#define MAX_SYMBOLS 256

extern symbol_t symbols[MAX_SYMBOLS];
extern int symbol_count;
extern unsigned short program_counter;

int find_symbol(char *name);
void add_symbol(char *name, unsigned short address);

#endif
