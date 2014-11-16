#ifndef PARSER_80IJUHQT

#define PARSER_80IJUHQT

#include "../containers/header.h"
#include <stdio.h>

/* function.c */
unsigned get_until_space(char *, char **); /* Don't use this, please. */
unsigned handle_args(_func_nfo *, char *); /* Used by parse_function */
_func_nfo *parse_function(char **); 	/* The argument must be preallocated, */
/* simply an address will trigger a segfault. This will modify the first pointer */
/* in the argument such that it starts exactly after the closing parentheses of */
/* the arguments of the function/method it was called to parse. */

/* class.c */
_class *parse_class(char **);

#endif /* end of include guard: PARSER_80IJUHQT */
