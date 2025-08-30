#ifndef UTILS_H
#define UTILS_H

#include "types.h"

int parse_number(char *str);
addr_mode_t detect_addressing_mode(char *operand);
int get_instruction_bytes(addr_mode_t mode);
char* trim_whitespace(char *str);

#endif
