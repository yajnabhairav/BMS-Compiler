#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Test input
    char code[] = "int main() { float x = 3.14; return 0; }";

    // Initialize the lexer
    struct Lexer lexer;
    Lexer_Init(&lexer, code, strlen(code));

    // Tokenize and print tokens
    while (true) {
        struct Token token = Lexer_PeekToken(&lexer);
        if (token.type == TOKEN_END_OF_FILE) break;
        PrintToken(token);
        Lexer_EatToken(&lexer);
    }

    return 0;
}
