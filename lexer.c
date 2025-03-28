#include "Lexer.h"
#include "ReportError.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEW_TYPE(type) ((struct type *) malloc(sizeof(struct type)))

typedef bool (*IsAllowedInSequenceFunction)(char *);

static bool IsAlphabetic(char c);
static bool IsDigit(char c);
static char PeekChar(struct Lexer *l);
static struct Token MakeToken(struct Lexer *l);
static void ReadSequence(struct Lexer *l, char *buffer, IsAllowedInSequenceFunction IsAllowed);
static enum TokenType TypeOfIdentifier(char *identifier);

static void AddToken(struct Lexer *l, struct Token token) {
    token.line = l->line;
    l->tokens[l->token_index] = token;
    l->token_index = (l->token_index + 1) % LEXER_TOKEN_CACHE_SIZE;
}

static void AddTokenWithType(struct Lexer *l, enum TokenType type) {
    struct Token token = MakeToken(l);
    token.type = type;
    AddToken(l, token);
}

static void EatChar(struct Lexer *l) {
    l->code_index += 1;
}

static void EatWhitespaceAndComments(struct Lexer *l) {
    bool is_done = false;
    while (!is_done) {
        switch (PeekChar(l)) {
            case '\n':
            case '\r': {
                l->line += 1;
                EatChar(l);
            } break;
            case ' ': {
                EatChar(l);
            } break;
            case '/': {
                if (l->code[l->code_index + 1] == '/') {
                    while (PeekChar(l) != '\n') {
                        EatChar(l);
                    }
                } else if (l->code[l->code_index + 1] == '*') {
                    while (!(PeekChar(l) == '*' && l->code[l->code_index + 1] == '/')) {
                        EatChar(l);
                    }
                    EatChar(l);
                    EatChar(l);
                } else {
                    is_done = true;
                }
            } break;
            default: {
                is_done = true;
            } break;
        }
    }
}

static struct Directive *FindDirectiveByIdentifier(struct Lexer *l, char *identifier) {
    for (int i = 0; i < l->directives.count; ++i) {
        struct Directive *directive = (struct Directive *) List_Get(&l->directives, i);
        if (strcmp(directive->identifier, identifier) == 0) {
            return directive;
        }
    }
    return NULL;
}

static bool IsAllowedInIdentifier(char *c) {
    return IsAlphabetic(*c) || IsDigit(*c) || *c == '_';
}

static bool IsAllowedInStringLiteral(char *c) {
    return *c != '"' && *c != '\n';
}

static bool IsNotNewline(char *c) {
    return *c != '\n';
}

static bool IsAlphabetic(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static bool IsDigit(char c) {
    return '0' <= c && c <= '9';
}

static struct Token MakeToken(struct Lexer *l) {
    struct Token token;
    token.line = l->line;
    token.location = l->code + l->code_index;
    return token;
}

static int NumCharsLeft(struct Lexer *l) {
    return l->code_length - l->code_index;
}

static char PeekChar(struct Lexer *l) {
    return l->code[l->code_index];
}

static void PreprocessDefine(struct Lexer *l) {
    struct Directive *directive = NEW_TYPE(Directive);
    directive->type = TOKEN_KEYWORD_DEFINE;

    ReadSequence(l, directive->identifier, IsAllowedInIdentifier);
    EatWhitespaceAndComments(l);
    ReadSequence(l, directive->value, IsNotNewline);
    List_Add(&l->directives, directive);
}

static void PreprocessInclude(struct Lexer *l) {
    struct Directive *directive = NEW_TYPE(Directive);
    directive->type = TOKEN_KEYWORD_INCLUDE;

    EatWhitespaceAndComments(l);
    if (PeekChar(l) == '"') {
        EatChar(l);
        ReadSequence(l, directive->value, IsAllowedInStringLiteral);
        if (PeekChar(l) != '"') {
            char *location = l->code + l->code_index;
            ReportErrorAt(l, location, "expected closing quote for include path");
        }
        EatChar(l);
    } else {
        char *location = l->code + l->code_index;
        ReportErrorAt(l, location, "expected include path");
    }
    List_Add(&l->directives, directive);
}

static void Preprocess(struct Lexer *l) {
    if (PeekChar(l) == '#') {
        EatChar(l);
        char identifier[TOKEN_MAX_IDENTIFIER_LENGTH];
        ReadSequence(l, identifier, IsAllowedInIdentifier);
        EatWhitespaceAndComments(l);

        enum TokenType keyword = TypeOfIdentifier(identifier);
        switch (keyword) {
            case TOKEN_KEYWORD_DEFINE: { PreprocessDefine(l); } break;
            case TOKEN_KEYWORD_INCLUDE: { PreprocessInclude(l); } break;
            default: {
                char *location = l->code + l->code_index;
                ReportErrorAt(l, location, "unknown preprocess directive");
            } break;
        }
    }
}

static void ReadSequence(struct Lexer *l, char *buffer, IsAllowedInSequenceFunction IsAllowed) {
    int length = 0;
    while (NumCharsLeft(l) > 0) {
        char *c = l->code + l->code_index;
        if (!IsAllowed(c)) {
            break;
        }
        buffer[length] = *c;
        length += 1;
        EatChar(l);
    }
    buffer[length] = '\0';
}

static enum TokenType TypeOfIdentifier(char *identifier) {
    if (strcmp(identifier, "char") == 0)    return TOKEN_KEYWORD_CHAR;
    if (strcmp(identifier, "define") == 0)  return TOKEN_KEYWORD_DEFINE;
    if (strcmp(identifier, "else") == 0)    return TOKEN_KEYWORD_ELSE;
    if (strcmp(identifier, "for") == 0)     return TOKEN_KEYWORD_FOR;
    if (strcmp(identifier, "int") == 0)     return TOKEN_KEYWORD_INT;
    if (strcmp(identifier, "if") == 0)      return TOKEN_KEYWORD_IF;
    if (strcmp(identifier, "include") == 0) return TOKEN_KEYWORD_INCLUDE;
    if (strcmp(identifier, "return") == 0)  return TOKEN_KEYWORD_RETURN;
    if (strcmp(identifier, "sizeof") == 0)  return TOKEN_KEYWORD_SIZEOF;
    if (strcmp(identifier, "struct") == 0)  return TOKEN_KEYWORD_STRUCT;
    if (strcmp(identifier, "while") == 0)   return TOKEN_KEYWORD_WHILE;
    return TOKEN_IDENTIFIER;
}

static struct Token *PopTokenQueue(struct Lexer *l) {
    struct Token *token = (struct Token *) List_Get(&l->token_queue, l->token_queue_tail);
    l->token_queue_tail += 1;
    return token;
}

static void ParseDirectiveValue(struct Lexer *l, char *value) {
    struct Lexer temp_l;
    Lexer_Init(&temp_l, value, (int) strlen(value));
    while (true) {
        struct Token token = Lexer_PeekToken(&temp_l);
        if (token.type == TOKEN_END_OF_FILE) {
            break;
        }
        struct Token *t = (struct Token *) malloc(sizeof(struct Token));
        t->int_value = token.int_value;
        t->line = token.line;
        t->location = token.location;
        strcpy(t->str_value, token.str_value);
        t->type = token.type;
        List_Add(&l->token_queue, t);
        Lexer_EatToken(&temp_l);
    }
}

void Lexer_EatToken(struct Lexer *l) {
    char c = PeekChar(l);
    while (c == '#') {
        Preprocess(l);
        EatWhitespaceAndComments(l);
        c = PeekChar(l);
    }

    if (l->token_queue_tail < l->token_queue.count) {
        struct Token *t = PopTokenQueue(l);
        AddToken(l, *t);
        return;
    } else if (l->token_queue_tail == l->token_queue.count) {
        l->token_queue_tail = 0;
        l->token_queue.count = 0;
    } else {
        assert(false);
    }

    EatWhitespaceAndComments(l);
    if (NumCharsLeft(l) == 0) {
        AddTokenWithType(l, TOKEN_END_OF_FILE);
        return;
    }

    struct Token token = MakeToken(l);
    c = PeekChar(l);
    switch (c) {
        case ',': { EatChar(l); token.type = TOKEN_COMMA; } break;
        case '.': { EatChar(l); token.type = TOKEN_DOT; } break;
        case ';': { EatChar(l); token.type = TOKEN_SEMICOLON; } break;
        case '(': { EatChar(l); token.type = TOKEN_LEFT_ROUND_BRACKET; } break;
        case ')': { EatChar(l); token.type = TOKEN_RIGHT_ROUND_BRACKET; } break;
        case '{': { EatChar(l); token.type = TOKEN_LEFT_CURLY_BRACKET; } break;
        case '}': { EatChar(l); token.type = TOKEN_RIGHT_CURLY_BRACKET; } break;
        case '[': { EatChar(l); token.type = TOKEN_LEFT_SQUARE_BRACKET; } break;
        case ']': { EatChar(l); token.type = TOKEN_RIGHT_SQUARE_BRACKET; } break;
        case '=': {
            EatChar(l);
            switch (PeekChar(l)) {
                case '=': { EatChar(l); token.type = TOKEN_2_EQUALS; } break;
                default: { token.type = TOKEN_EQUALS; } break;
            }
        } break;
        case '*': { EatChar(l); token.type = TOKEN_STAR; } break;
        case '/': { EatChar(l); token.type = TOKEN_SLASH; } break;
        case '!': {
            EatChar(l);
            switch (PeekChar(l)) {
                case '=': { EatChar(l); token.type = TOKEN_EXCLAMATION_MARK_EQUALS; } break;
                default: { token.type = TOKEN_EXCLAMATION_MARK; } break;
            }
        } break;
        case '%': { EatChar(l); token.type = TOKEN_PERCENTAGE; } break;
        case '+': { EatChar(l); token.type = TOKEN_PLUS; } break;
        case '&': { EatChar(l); token.type = TOKEN_AMPERSAND; } break;
        case '-': { EatChar(l); token.type = TOKEN_MINUS; } break;
        case '<': {
            EatChar(l);
            if (PeekChar(l) == '=') {
                EatChar(l);
                token.type = TOKEN_LESS_THAN_EQUALS;
            } else {
                token.type = TOKEN_LESS_THAN;
            }
        } break;
        case '>': {
            EatChar(l);
            if (PeekChar(l) == '=') {
                EatChar(l);
                token.type = TOKEN_GREATER_THAN_EQUALS;
            } else {
                token.type = TOKEN_GREATER_THAN;
            }
        } break;
        case '"': {
            EatChar(l);
            ReadSequence(l, token.str_value, IsAllowedInStringLiteral);
            token.type = TOKEN_LITERAL_STRING;
            if (PeekChar(l) != '"') {
                char *location = l->code + l->code_index;
                ReportErrorAt(l, location, "expected end of string \"");
            }
            EatChar(l);
        } break;
        case '\'': {
            EatChar(l);
            token.str_value[0] = PeekChar(l);
            token.str_value[1] = '\0';
            token.type = TOKEN_LITERAL_CHAR;
            EatChar(l);
            if (PeekChar(l) != '\'') {
                char *location = l->code + l->code_index;
                ReportErrorAt(l, location, "expected closing single quote");
            }
            EatChar(l);
        } break;
        default: {
            if (IsAlphabetic(c) || c == '_') {
                ReadSequence(l, token.str_value, IsAllowedInIdentifier);
                struct Directive *directive = FindDirectiveByIdentifier(l, token.str_value);
                if (directive) {
                    ParseDirectiveValue(l, directive->value);
                    if (l->token_queue_tail < l->token_queue.count) {
                        struct Token *t = PopTokenQueue(l);
                        AddToken(l, *t);
                        return;
                    } else {
                        ReportInternalError("error with directive???");
                    }
                } else {
                    token.type = TypeOfIdentifier(token.str_value);
                }
            } else if (IsDigit(c)) {
                char *p = l->code + l->code_index;
                char *q = p;
                token.int_value = strtoul(p, &p, 10);
                if (*p == '.') {
                    p++;
                    while (IsDigit(*p)) {
                        p++;
                    }
                    token.type = TOKEN_LITERAL_FLOAT;
                } else {
                    token.type = TOKEN_LITERAL_NUMBER;
                }
                int len = (int) (p - q);
                l->code_index += len;
            } else {
                char *location = l->code + l->code_index;
                ReportErrorAt(l, location, "unknown character");
            }
        } break;
    }

    AddToken(l, token);
}

void Lexer_Init(struct Lexer *l, char *code, int code_len) {
    List_Init(&l->token_queue);
    List_Init(&l->directives);
    l->code = code;
    l->code_index = 0;
    l->code_length = code_len;
    l->line = 1;
    l->token_index = 0;
    l->token_queue_tail = 0;
    for (int i = 0; i < LEXER_TOKEN_CACHE_SIZE; ++i) {
        Lexer_EatToken(l);
    }
}

struct Token Lexer_PeekToken(struct Lexer *l) {
    return Lexer_PeekToken2(l, 0);
}

struct Token Lexer_PeekToken2(struct Lexer *l, int offset) {
    int index = (l->token_index + offset) % LEXER_TOKEN_CACHE_SIZE;
    return l->tokens[index];
}
