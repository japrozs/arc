#ifndef ERRORS_H
#define ERRORS_H

#include "arc.h"
#include "parser.h"

void error_at_current(parser_t *parser, const char *msg, char *help_msg);

#endif // ERRORS_H
