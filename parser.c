#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token types
typedef enum {
    TOKEN_INT, TOKEN_ID, TOKEN_ASSIGN, TOKEN_SEMI, TOKEN_PLUS, TOKEN_MINUS,
    TOKEN_MUL, TOKEN_DIV, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[50];
} Token;

// Global token pointer
Token current_token;

// Function to get the next token (Dummy Lexer for now)
Token get_next_token() {
    Token token;
    token.type = TOKEN_EOF;
    strcpy(token.value, "");
    return token;
}

// Match function to consume expected token
void match(TokenType expected) {
    if (current_token.type == expected) {
        current_token = get_next_token();
    } else {
        printf("Syntax Error: Unexpected token %s\n", current_token.value);
        exit(EXIT_FAILURE);
    }
}

// Forward declarations
void expression();

// Parse factor (NUMBER | IDENTIFIER | (EXPRESSION))
void factor() {
    if (current_token.type == TOKEN_INT || current_token.type == TOKEN_ID) {
        printf("Parsed Factor: %s\n", current_token.value);
        match(current_token.type);
    } else if (current_token.type == TOKEN_LPAREN) {
        match(TOKEN_LPAREN);
        expression();
        match(TOKEN_RPAREN);
    } else {
        printf("Syntax Error: Expected number or identifier\n");
        exit(EXIT_FAILURE);
    }
}

// Parse term (factor ((MUL | DIV) factor)*)
void term() {
    factor();
    while (current_token.type == TOKEN_MUL || current_token.type == TOKEN_DIV) {
        printf("Parsed Operator: %s\n", current_token.value);
        match(current_token.type);
        factor();
    }
}

// Parse expression (term ((PLUS | MINUS) term)*)
void expression() {
    term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        printf("Parsed Operator: %s\n", current_token.value);
        match(current_token.type);
        term();
    }
}

// Parse assignment statement (ID = EXPRESSION;)
void assignment() {
    if (current_token.type == TOKEN_ID) {
        printf("Parsed Identifier: %s\n", current_token.value);
        match(TOKEN_ID);
        match(TOKEN_ASSIGN);
        expression();
        match(TOKEN_SEMI);
        printf("Parsed Assignment Statement\n");
    } else {
        printf("Syntax Error: Expected identifier at start of assignment\n");
        exit(EXIT_FAILURE);
    }
}

// Main parser function
void parse() {
    current_token = get_next_token(); // Initialize first token
    assignment();
    if (current_token.type != TOKEN_EOF) {
        printf("Syntax Error: Unexpected token at end\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    parse();
    printf("Parsing Completed Successfully!\n");
    return 0;
}

