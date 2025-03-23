#ifndef BMS_TOKEN_H
#define BMS_TOKEN_H

#define TOKEN_MAX_IDENTIFIER_LENGTH 64

enum TokenType {
    // Others
    TOKEN_END_OF_FILE,
    TOKEN_IDENTIFIER,

    // Literals
    TOKEN_LITERAL_NUMBER,
    TOKEN_LITERAL_STRING,
    TOKEN_LITERAL_FLOAT,
    TOKEN_LITERAL_CHAR,

    // Signs
    TOKEN_COMMA,                    // ,
    TOKEN_DOT,                      // .
    TOKEN_SEMICOLON,                // ;
    TOKEN_LEFT_ROUND_BRACKET,       // (
    TOKEN_RIGHT_ROUND_BRACKET,      // )
    TOKEN_LEFT_CURLY_BRACKET,       // {
    TOKEN_RIGHT_CURLY_BRACKET,      // }
    TOKEN_LEFT_SQUARE_BRACKET,      // [
    TOKEN_RIGHT_SQUARE_BRACKET,     // ]

    TOKEN_EQUALS,                   // =
    TOKEN_2_EQUALS,                 // ==
    TOKEN_STAR,                     // *
    TOKEN_EXCLAMATION_MARK,         // !
    TOKEN_EXCLAMATION_MARK_EQUALS,  // !=
    TOKEN_SLASH,                    // /
    TOKEN_PERCENTAGE,               // %
    TOKEN_PLUS,                     // +
    TOKEN_AMPERSAND,                // &
    TOKEN_MINUS,                    // -

    TOKEN_LESS_THAN,                // <
    TOKEN_LESS_THAN_EQUALS,         // <=
    TOKEN_GREATER_THAN,             // >
    TOKEN_GREATER_THAN_EQUALS,      // >=

    // Keywords
    TOKEN_KEYWORD_CHAR,
    TOKEN_KEYWORD_DEFINE,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_INCLUDE,          // Added
    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_SIZEOF,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_WHILE,

    // Count
    TOKEN_COUNT,
};

struct Token {
    int line;                       // Line number in the source code
    char *location;                 // Pointer to the token's location in the source code
    enum TokenType type;            // Type of the token
    union {
        int int_value;              // For integer literals
        float float_value;          // For floating-point literals
        char str_value[TOKEN_MAX_IDENTIFIER_LENGTH]; // For identifiers and string literals
    };
};

// Function prototypes
char *TokenTypeToStr(enum TokenType type); // Convert token type to string
void PrintToken(struct Token token);        // Print token details

#endif // BMS_TOKEN_H
