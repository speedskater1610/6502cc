#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

// Usage helper
void print_usage(const char* prog_name) {
    printf("Usage: %s [-o output_file] assembly_file\n", prog_name);
    printf("  -o output_file   Write assembled hex to a file\n");
    printf("If no output file is specified, hex is printed to stdout.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *input_file = NULL;
    const char *output_file = NULL;

    // Parse flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[i + 1];
            i++; // skip next arg since it's the filename
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        } else {
            printf("Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!input_file) {
        printf("No assembly file specified.\n");
        print_usage(argv[0]);
        return 1;
    }

    // Read input assembly file
    FILE *f = fopen(input_file, "r");
    if (!f) {
        perror("Failed to open input file");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *assembly_code = malloc(filesize + 1);
    if (!assembly_code) {
        perror("Memory allocation failed");
        fclose(f);
        return 1;
    }

    fread(assembly_code, 1, filesize, f);
    assembly_code[filesize] = '\0';
    fclose(f);

    // Assemble
    char *hex_output = assemble_6502(assembly_code);
    free(assembly_code);

    // Output
    if (output_file) {
        FILE *out = fopen(output_file, "w");
        if (!out) {
            perror("ERROR - Failed to open output file; 0x");
            return 1;
        }
        fprintf(out, "%s\n", hex_output);
        fclose(out);
        printf("Hex output written to %s\n", output_file);
    } else {
        printf("Assembled hex:\n%s\n", hex_output);
    }

    return 0;
}
