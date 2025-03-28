#ifndef BMS_ASSEMBLY_H
#define BMS_ASSEMBLY_H

#include "Register.h"  // Contains definitions for registers and primitive types
#include <stdio.h>     // For FILE* and fprintf

// Function declarations
void Add(char *destination, char *source);
void Call(char *label);
void Comment(char *comment);
void Compare(char *a, char *b, char *comparison);
void Div(char *operand);
void Jmp(char *label);
void Label(char *name);
void Lea(char *dest, int rbp_offset);
void LoadMem(enum PrimitiveType primtype);
void Mov(char *destination, char *source);
void MovImm(char *destination, int value);
void Mul(char *destination, char *source);
void Neg(char *destination);
void Pop(char *destination);
void Push(char *source);
void RestoreStackFrame();
void SetOutput(FILE *file);
void SetupAssemblyFile();
void SetupStackFrame(int stack_size);
void Sub(char *destination, char *source);
void WriteMemOffset(int rbp_offset, int reg_idx, enum PrimitiveType primtype);
void WriteMemToReg(char *dest, char *src);

#endif // BMS_ASSEMBLY_H
