#include "ReportError.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// Helper function to report an error with a formatted message
static void ReportError(char *code, const char *location, const char *format, va_list args) {
    // Calculate the position of the error in the source code
    int position = (int) (location - code);

    // Print the error message
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    // Print the line of code where the error occurred
    fprintf(stderr, "%s\n", code);

    // Print a caret (^) under the error location
    fprintf(stderr, "%*s", position, "");
    fprintf(stderr, "^\n");

    // Exit the program with an error code
    exit(1);
}

// Report an internal error
void ReportInternalError(const char *format, ...) {
    va_list args;
    va_start(args, format);

    fprintf(stderr, "Internal Error: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(1);
}

// Report an error at a specific location in the source code
void ReportErrorAt(struct Lexer *l, const char *location, const char *format, ...) {
    va_list args;
    va_start(args, format);
    ReportError(l->code, location, format, args);
    va_end(args);
}

// Report an error at the location of a specific token
void ReportErrorAtToken(struct Lexer *l, struct Token token, const char *format, ...) {
    va_list args;
    va_start(args, format);
    ReportError(l->code, token.location, format, args);
    va_end(args);
}
