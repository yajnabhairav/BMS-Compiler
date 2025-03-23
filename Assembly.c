#include "Assembly.h"
#include <assert.h>

static FILE *f;  // File pointer for output

// Function implementations
void Add(char *destination, char *source) {
    fprintf(f, "  add %s, %s\n", destination, source);
}

void Call(char *label) {
    fprintf(f, "  call %s\n", label);
}

void Comment(char *comment) {
    fprintf(f, "  ; %s\n", comment);
}

void Compare(char *a, char *b, char *comparison) {
    fprintf(f,
        "  cmp %s, %s\n"
        "  %s al\n",  // Store comparison result in 'al' (lower 8 bits of rax)
        a, b,
        comparison
    );
}

void Div(char *operand) {
    fprintf(f,
        "  cqo\n"          // Prepare for signed division (convert quadword to octaword)
        "  idiv %s\n",     // Signed division
        operand
    );
}

void Jmp(char *label) {
    fprintf(f, "  jmp %s\n", label);
}

void Label(char *name) {
    fprintf(f, "%s:\n", name);
}

void Lea(char *dest, int rbp_offset) {
    fprintf(f, "  lea %s, [rbp - %d]\n", dest, rbp_offset);
}

void LoadMem(enum PrimitiveType primtype) {
    if (primtype == PRIMTYPE_CHAR) {
        fprintf(f, "  movzx rax, %s [rax]\n", size[primtype]);  // Zero-extend for char
    }
    else {
        fprintf(f, "  mov %s, %s [rax]\n", rax[primtype], size[primtype]);
    }
}

void Mov(char *destination, char *source) {
    fprintf(f, "  mov %s, %s\n", destination, source);
}

void MovImm(char *destination, int value) {
    fprintf(f, "  mov %s, %d\n", destination, value);
}

void Mul(char *destination, char *source) {
    fprintf(f, "  imul %s, %s\n", destination, source);
}

void Neg(char *destination) {
    fprintf(f, "  neg %s\n", destination);
}

void Pop(char *destination) {
    fprintf(f, "  pop %s\n", destination);
}

void Push(char *source) {
    fprintf(f, "  push %s\n", source);
}

void RestoreStackFrame() {
    fprintf(f,
        "  mov rsp, rbp\n"  // Restore stack pointer
        "  pop rbp\n"        // Restore base pointer
        "  ret\n"            // Return from function
    );
}

void SetOutput(FILE *file) {
    f = file;  // Set the output file
}

void SetupAssemblyFile() {
    fprintf(f,
        "bits 64\n"      // 64-bit mode
        "default rel\n"   // Default to RIP-relative addressing
        "\n"
    );
}

void SetupStackFrame(int stack_size) {
    fprintf(f,
        "  push rbp\n"    // Save old base pointer
        "  mov rbp, rsp\n" // Set new base pointer
    );

    if (stack_size > 0) {
        fprintf(f, "  sub rsp, %d\n", stack_size);  // Allocate stack space
    }
}

void Sub(char *destination, char *source) {
    fprintf(f, "  sub %s, %s\n", destination, source);
}

void WriteMemOffset(int rbp_offset, int reg_idx, enum PrimitiveType primtype) {
    assert(0 <= reg_idx && reg_idx < 4);  // Ensure reg_idx is valid
    char **param_reg = param_regs[reg_idx];  // Get register for the given index
    char *reg = param_reg[primtype];         // Get register for the given primitive type
    fprintf(f, "  mov [rbp - %d], %s\n", rbp_offset, reg);  // Write to memory
}

void WriteMemToReg(char *dest, char *src) {
    fprintf(f, "  mov [%s], %s\n", dest, src);  // Write to memory
}
