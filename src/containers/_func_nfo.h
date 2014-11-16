#ifndef _FUNC_NFO_V2Z1ITRZ

#define _FUNC_NFO_V2Z1ITRZ

#define FUNC_TEMPLATE 1

#include <stdlib.h>
#include <string.h>

typedef struct _func_nfo {
	char *name; /* Function name, nil terminated */
	char *ret_t; /* Function return type, nil terminated */
	char **arg_type; /* Arguments' data types, see length at arg_len*/
	char **args; /* Arguments, see length at arg_len*/
	unsigned arg_len; /* Number of arguments the function has*/
	unsigned long long int flags; /* Guess*/
} _func_nfo;

_func_nfo 	*create_func_nfo(void); /* Constructor */
void 		destroy_func_nfo(_func_nfo *); /* Destructor */

/* Setters, read the part before _func_nfo */
void 		set_name_func_nfo(_func_nfo *, const char *, unsigned);
void 		set_ret_t_func_nfo(_func_nfo *, const char *, unsigned);
void 		add_arg_func_nfo(_func_nfo *, const char *, const char *);

/* Getters, read the part before _func_nfo */
const char 	*get_name_func_nfo(_func_nfo *);
const char 	*get_ret_t_func_nfo(_func_nfo *);
const char 	**get_arg_type_func_nfo(_func_nfo *);
const char 	**get_args_func_nfo(_func_nfo *);
const unsigned 	get_arg_len_func_nfo(_func_nfo *);
const long long int get_flags_func_nfo(_func_nfo *);

#endif /* end of include guard: _FUNC_NFO_V2Z1ITRZ */
