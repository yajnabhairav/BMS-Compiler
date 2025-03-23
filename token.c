#include "Token.h"
#include <stdio.h>

// Convert token type to string
char *TokenTypeToStr(enum TokenType type) {
#define RETURN_STR(x) case x: return #x;
    switch (type) {
        RETURN_STR(TOKEN_END_OF_FILE);
        RETURN_STR(TOKEN_IDENTIFIER);
        RETURN_STR(TOKEN_LITERAL_NUMBER);
        RETURN_STR(TOKEN_LITERAL_STRING);
        RETURN_STR(TOKEN_LITERAL_FLOAT);  // Added
        RETURN_STR(TOKEN_LITERAL_CHAR);   // Added
        RETURN_STR(TOKEN_COMMA);
        RETURN_STR(TOKEN_SEMICOLON);
        RETURN_STR(TOKEN_DOT);
        RETURN_STR(TOKEN_LEFT_ROUND_BRACKET);
        RETURN_STR(TOKEN_RIGHT_ROUND_BRACKET);
        RETURN_STR(TOKEN_LEFT_CURLY_BRACKET);
        RETURN_STR(TOKEN_RIGHT_CURLY_BRACKET);
        RETURN_STR(TOKEN_LEFT_SQUARE_BRACKET);
        RETURN_STR(TOKEN_RIGHT_SQUARE_BRACKET);
        RETURN_STR(TOKEN_EQUALS);
        RETURN_STR(TOKEN_2_EQUALS);
        RETURN_STR(TOKEN_STAR);
        RETURN_STR(TOKEN_EXCLAMATION_MARK_EQUALS);
        RETURN_STR(TOKEN_SLASH);
        RETURN_STR(TOKEN_PERCENTAGE);
        RETURN_STR(TOKEN_PLUS);
        RETURN_STR(TOKEN_AMPERSAND);
        RETURN_STR(TOKEN_MINUS);
        RETURN_STR(TOKEN_LESS_THAN);
        RETURN_STR(TOKEN_LESS_THAN_EQUALS);
        RETURN_STR(TOKEN_GREATER_THAN);
        RETURN_STR(TOKEN_GREATER_THAN_EQUALS);
        RETURN_STR(TOKEN_KEYWORD_CHAR);
        RETURN_STR(TOKEN_KEYWORD_DEFINE);
        RETURN_STR(TOKEN_KEYWORD_ELSE);
        RETURN_STR(TOKEN_KEYWORD_FOR);
        RETURN_STR(TOKEN_KEYWORD_INT);
        RETURN_STR(TOKEN_KEYWORD_IF);
        RETURN_STR(TOKEN_KEYWORD_RETURN);
        RETURN_STR(TOKEN_KEYWORD_SIZEOF);
        RETURN_STR(TOKEN_KEYWORD_STRUCT);
        RETURN_STR(TOKEN_KEYWORD_WHILE);
        default: return "UNKNOWN_TOKEN";
    }
#undef RETURN_STR
}

// Print token details
void PrintToken(struct Token token) {
    printf("<Token\n");
    printf("  line=\"%d\"\n", token.line);
    printf("  location=\"%p\"\n", token.location);
    printf("  type=\"%s\"\n", TokenTypeToStr(token.type));

    // Print token value based on type
    switch (token.type) {
        case TOKEN_LITERAL_NUMBER:
            printf("  int_value=\"%d\"\n", token.int_value);
            break;
        case TOKEN_LITERAL_FLOAT:
            printf("  float_value=\"%f\"\n", token.float_value);
            break;
        case TOKEN_LITERAL_STRING:
        case TOKEN_IDENTIFIER:
            printf("  str_value=\"%s\"\n", token.str_value);
            break;
        default:
            // No value to print for other token types
            break;
    }
    printf(">\n");
}
