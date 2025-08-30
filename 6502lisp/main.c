#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../assembler/assembler.h"
#include "../external/mpc.h"

// Global output buffer for generated assembly
char asm_output[4096];
int asm_pos = 0;
int temp_counter = 0;
int label_counter = 0;

// Stack management for 6502 (using zero page)
int stack_ptr = 0x80;  // Start of our variable stack in zero page

// Symbol table for variables
typedef struct {
    char name[32];
    unsigned char addr;  // Zero page address
} var_t;

var_t variables[32];
int var_count = 0;

// Function to emit assembly code
void emit(const char* format, ...) {
    va_list args;
    va_start(args, format);
    asm_pos += vsprintf(asm_output + asm_pos, format, args);
    asm_pos += sprintf(asm_output + asm_pos, "\n");
    va_end(args);
}

// Find or create variable
unsigned char get_var_addr(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].addr;
        }
    }
    
    // Create new variable
    if (var_count < 32) {
        strcpy(variables[var_count].name, name);
        variables[var_count].addr = stack_ptr++;
        return variables[var_count++].addr;
    }
    
    return 0x80; // Default fallback
}

// Generate unique labels
char* gen_label() {
    static char label[16];
    sprintf(label, "L%d", label_counter++);
    return label;
}

// Forward declaration
void compile_expr(mpc_ast_t* ast);

// Compile arithmetic operations
void compile_arith(mpc_ast_t* ast, const char* op) {
    if (ast->children_num < 3) return;
    
    // Compile first operand (result in accumulator)
    compile_expr(ast->children[1]);
    
    // Save to temp location
    unsigned char temp_addr = stack_ptr++;
    emit("STA $%02X", temp_addr);
    
    // Compile second operand
    compile_expr(ast->children[2]);
    
    // Perform operation
    if (strcmp(op, "+") == 0) {
        emit("CLC");
        emit("ADC $%02X", temp_addr);
    } else if (strcmp(op, "-") == 0) {
        // A = temp - A, so we need: A = temp, then A = A - operand
        unsigned char temp2 = stack_ptr++;
        emit("STA $%02X", temp2);   // Save second operand
        emit("LDA $%02X", temp_addr); // Load first operand
        emit("SEC");
        emit("SBC $%02X", temp2);   // Subtract second operand
    } else if (strcmp(op, "*") == 0) {
        // Simple multiplication by repeated addition
        unsigned char counter = stack_ptr++;
        unsigned char result = stack_ptr++;
        char* loop_label = gen_label();
        char* end_label = gen_label();
        
        emit("STA $%02X", counter);  // Store multiplier
        emit("LDA #$00");
        emit("STA $%02X", result);   // Clear result
        emit("LDA $%02X", counter);
        emit("BEQ %s", end_label);   // If multiplier is 0, done
        
        emit("%s:", loop_label);
        emit("LDA $%02X", result);
        emit("CLC");
        emit("ADC $%02X", temp_addr); // Add multiplicand
        emit("STA $%02X", result);
        emit("DEC $%02X", counter);
        emit("BNE %s", loop_label);
        
        emit("%s:", end_label);
        emit("LDA $%02X", result);   // Load result into A
    }
    
    stack_ptr = temp_addr; // Reset temp stack
}

// Compile comparison operations
void compile_comparison(mpc_ast_t* ast, const char* op) {
    if (ast->children_num < 3) return;
    
    char* true_label = gen_label();
    char* end_label = gen_label();
    
    // Compile first operand
    compile_expr(ast->children[1]);
    unsigned char temp_addr = stack_ptr++;
    emit("STA $%02X", temp_addr);
    
    // Compile second operand
    compile_expr(ast->children[2]);
    
    // Compare
    emit("CMP $%02X", temp_addr);
    
    // Generate appropriate branch
    if (strcmp(op, "=") == 0) {
        emit("BEQ %s", true_label);
    } else if (strcmp(op, "<") == 0) {
        emit("BCC %s", true_label);
    } else if (strcmp(op, ">") == 0) {
        emit("BEQ %s", end_label);  // Skip if equal
        emit("BCC %s", end_label);  // Skip if less
        emit("JMP %s", true_label); // Must be greater
    }
    
    // False case
    emit("LDA #$00");
    emit("JMP %s", end_label);
    
    // True case  
    emit("%s:", true_label);
    emit("LDA #$FF");
    
    emit("%s:", end_label);
    stack_ptr = temp_addr; // Reset temp stack
}

// Compile if statement
void compile_if(mpc_ast_t* ast) {
    if (ast->children_num < 4) return;
    
    char* else_label = gen_label();
    char* end_label = gen_label();
    
    // Compile condition
    compile_expr(ast->children[1]);
    
    // Test result
    emit("CMP #$00");
    emit("BEQ %s", else_label);
    
    // Then clause
    compile_expr(ast->children[2]);
    emit("JMP %s", end_label);
    
    // Else clause
    emit("%s:", else_label);
    if (ast->children_num > 3) {
        compile_expr(ast->children[3]);
    } else {
        emit("LDA #$00");  // Default to 0 if no else clause
    }
    
    emit("%s:", end_label);
}

// Compile let binding
void compile_let(mpc_ast_t* ast) {
    if (ast->children_num < 3) return;
    
    // Get variable name from bindings
    mpc_ast_t* bindings = ast->children[1];
    if (bindings->children_num >= 2) {
        char* var_name = bindings->children[0]->contents;
        
        // Compile the value expression
        compile_expr(bindings->children[1]);
        
        // Store in variable
        unsigned char addr = get_var_addr(var_name);
        emit("STA $%02X", addr);
    }
    
    // Compile body
    if (ast->children_num > 2) {
        compile_expr(ast->children[2]);
    }
}

// Main expression compiler
void compile_expr(mpc_ast_t* ast) {
    if (!ast) return;
    
    // Handle atoms (numbers and symbols)
    if (ast->children_num == 0) {
        if (ast->contents[0] >= '0' && ast->contents[0] <= '9') {
            // Number literal
            int value = atoi(ast->contents);
            emit("LDA #$%02X", value & 0xFF);
        } else {
            // Variable reference
            unsigned char addr = get_var_addr(ast->contents);
            emit("LDA $%02X", addr);
        }
        return;
    }
    
    // Handle S-expressions
    if (ast->children_num > 0) {
        mpc_ast_t* op = ast->children[0];
        
        if (strcmp(op->contents, "+") == 0) {
            compile_arith(ast, "+");
        } else if (strcmp(op->contents, "-") == 0) {
            compile_arith(ast, "-");
        } else if (strcmp(op->contents, "*") == 0) {
            compile_arith(ast, "*");
        } else if (strcmp(op->contents, "=") == 0) {
            compile_comparison(ast, "=");
        } else if (strcmp(op->contents, "<") == 0) {
            compile_comparison(ast, "<");
        } else if (strcmp(op->contents, ">") == 0) {
            compile_comparison(ast, ">");
        } else if (strcmp(op->contents, "if") == 0) {
            compile_if(ast);
        } else if (strcmp(op->contents, "let") == 0) {
            compile_let(ast);
        } else if (strcmp(op->contents, "print") == 0) {
            // Simple print - store value at $0200 (typical output location)
            if (ast->children_num > 1) {
                compile_expr(ast->children[1]);
                emit("STA $0200");
            }
        }
    }
}

// Main Lisp to 6502 compiler function
char* lisp_to_6502(const char* lisp_code) {
    // Reset global state
    asm_pos = 0;
    temp_counter = 0;
    label_counter = 0;
    var_count = 0;
    stack_ptr = 0x80;
    memset(asm_output, 0, sizeof(asm_output));
    
    // Create MPC parsers
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lisp = mpc_new("lisp");
    
    // Define Lisp grammar using mpca_lang
    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                    \
        number : /-?[0-9]+/ ;                               \
        symbol : /[a-zA-Z_+\\-*\\/\\\\=<>!&][a-zA-Z0-9_+\\-*\\/\\\\=<>!&]*/ ; \
        sexpr  : '(' <expr>* ')' ;                          \
        expr   : <number> | <symbol> | <sexpr> ;            \
        lisp   : /^/ <expr>* /$/ ;                          \
        ", Number, Symbol, Sexpr, Expr, Lisp);
    
    // Parse the Lisp code
    mpc_result_t result;
    if (mpc_parse("input", (char*)lisp_code, Lisp, &result)) {
        // Add program header
        emit("; Generated 6502 assembly from Lisp");
        emit(".org $0800");
        emit("start:");
        
        // Compile each top-level expression
        mpc_ast_t* ast = result.output;
        for (int i = 0; i < ast->children_num; i++) {
            if (ast->children[i]->children_num > 0) {
                compile_expr(ast->children[i]);
            }
        }
        
        // Add program footer
        emit("end:");
        emit("BRK");
        
        mpc_ast_delete(result.output);
    } else {
        sprintf(asm_output, "; Parse error: %s", result.error);
        free(result.error);
    }
    
    // Cleanup parsers
    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lisp);
    
    return asm_output;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-s|-o] <input.lisp> [output]\n", argv[0]);
        return 1;
    }

    int to_asm = 0;   // -s
    int to_obj = 0;   // -o
    const char *input_file = NULL;
    const char *output_file = NULL;

    // Parse args
    if (strcmp(argv[1], "-s") == 0) {
        to_asm = 1;
    } else if (strcmp(argv[1], "-o") == 0) {
        to_obj = 1;
    } else {
        fprintf(stderr, "Unknown flag: %s\n", argv[1]);
        return 1;
    }

    input_file = argv[2];
    if (argc >= 4) {
        output_file = argv[3];
    } else {
        output_file = to_asm ? "out.asm" : "out.o";
    }

    // Read input file
    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("fopen input");
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char *source = malloc(fsize + 1);
    fread(source, 1, fsize, fin);
    fclose(fin);
    source[fsize] = '\0';

    // Compile Lisp → 6502 assembly
    char *asm_code = lisp_to_6502(source);

    if (to_asm) {
        // Write assembly text file
        FILE *fout = fopen(output_file, "w");
        if (!fout) {
            perror("fopen output");
            free(source);
            return 1;
        }
        fprintf(fout, "%s", asm_code);
        fclose(fout);
        printf("Wrote assembly to %s\n", output_file);
    } else if (to_obj) {
        // Assemble into machine code  
        char *hex_output = assemble_6502(asm_code);
        
        // Convert hex string to binary
        unsigned char machine_code[4096];
        int code_size = 0;
        char *hex_ptr = hex_output;
        
        while (*hex_ptr && code_size < 4096) {
            if (*hex_ptr == ' ') {
                hex_ptr++;
                continue;
            }
            
            // Parse two hex digits
            unsigned int byte_val;
            if (sscanf(hex_ptr, "%2X", &byte_val) == 1) {
                machine_code[code_size++] = (unsigned char)byte_val;
                hex_ptr += 2;
            } else {
                break;
            }
        }

        FILE *fout = fopen(output_file, "wb");
        if (!fout) {
            perror("fopen output");
            free(source);
            return 1;
        }
        fwrite(machine_code, 1, code_size, fout);
        fclose(fout);
        printf("Wrote object code to %s (%d bytes)\n", output_file, code_size);
    }

    free(source);
    return 0;
}
