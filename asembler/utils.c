#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "symbols.h"

char* trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int parse_number(char *str) {
    if (!str) return 0;
    if (str[0] == '$') return strtol(str + 1, NULL, 16);
    if (str[0] == '#' && str[1] == '$') return strtol(str + 2, NULL, 16);
    if (str[0] == '#') return strtol(str + 1, NULL, 10);
    if (str[0] == '%') return strtol(str + 1, NULL, 2);

    int idx = find_symbol(str);
    if (idx >= 0) return symbols[idx].address;

    return strtol(str, NULL, 10);
}

addr_mode_t detect_addressing_mode(char *operand) {
    if (!operand || strlen(operand) == 0) return IMP;
    operand = trim_whitespace(operand);
    if (operand[0] == '#') return IMM;
    if (strstr(operand, ",X")) {
        if (operand[0] == '(' && operand[strlen(operand)-3] == ')') return IZX;
        if (strlen(operand) <= 5) return ZPX;
        return ABX;
    }
    if (strstr(operand, ",Y")) {
        if (operand[0] == '(' && strstr(operand, "),Y")) return IZY;
        if (strlen(operand) <= 5) return ZPY;
        return ABY;
    }
    if (operand[0] == '(' && operand[strlen(operand)-1] == ')') return IND;

    int value = parse_number(operand);
    if (value <= 0xFF && operand[0] == '$' && strlen(operand) <= 3) return ZPG;

    return ABS;
}

int get_instruction_bytes(addr_mode_t mode) {
    switch (mode) {
        case IMP: return 1;
        case IMM: case ZPG: case ZPX: case ZPY: case IZX: case IZY: case REL: return 2;
        case ABS: case ABX: case ABY: case IND: return 3;
        default: return 1;
    }
}
