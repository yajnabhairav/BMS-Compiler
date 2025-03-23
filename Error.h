#ifndef BMS_REPORT_ERROR_H
#define BMS_REPORT_ERROR_H

#include "Lexer.h"
#include <stdarg.h>
#include <stdbool.h>

// Function prototypes

// Report an internal error (e.g., for bugs in the compiler itself)
void ReportInternalError(const char *format, ...);

// Report an error at a specific location in the source code
void ReportErrorAt(struct Lexer *l, const char *location, const char *format, ...);

// Report an error at the location of a specific token
void ReportErrorAtToken(struct Lexer *l, struct Token token, const char *format, ...);

#endif // BMS_REPORT_ERROR_H
