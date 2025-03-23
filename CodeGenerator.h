#ifndef BMS_CODE_GENRATOR_X86_H
#define BMS_CODE_GENRATOR_X86_H

#include "AstNode.h"
#include "List.h"
#include <stdbool.h>
#include <stdio.h>

// Function to generate x86 assembly code from the AST
void CodeGeneratorX86_GenerateCode(FILE *asm_file, struct TranslationUnit *t_unit);

#endif // MINIC_CODE_GENRATOR_X86_H
