#ifndef BMS_PARSER_H
#define BMS_PARSER_H

#include "Lexer.h"
#include "List.h"
#include "AstNode.h"
#include <stdbool.h>

// Function to create an AST from the lexer output
struct TranslationUnit *Parser_MakeAst(struct Lexer *lexer);

#endif // BMS_PARSER_H
