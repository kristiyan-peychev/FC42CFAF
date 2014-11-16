#ifndef _METHOD_NFO_QL95EGPM

#define _METHOD_NFO_QL95EGPM

#define METHOD_PURE_VIRTUAL 	1  /* <...> (...) = 0?? */
#define METHOD_CONST_RET 	2  /* <CONST?> ... */
#define METHOD_CONST_MOD 	4  /* <ret> <m_name>(<args>) <CONST?>... */
#define METHOD_PRIVATE 		8  /* info about where the */
#define METHOD_PROTECTED 	16 /* method is in the class */
#define METHOD_PUBLIC 		32 /* it belongs to */

#include "header.h"

typedef struct _method_nfo {
	_func_nfo *func;
	_class *super;
	unsigned long long int flags;
} _method_nfo;

_method_nfo 	*create_method_nfo(_class *); /* Constructor, pass it the method's class in the argument */
void 		destroy_method_nfo(_method_nfo *); /* Descturctor */
_method_nfo 	*copy_method_nfo(_method_nfo *); /* Constructor */

/* Setters */
void 		set_method_nfo(_method_nfo *, _func_nfo *); /* Set the function part of the method */
void 		set_super_method_nfo(_method_nfo *, _class *); /* what */

/* Getters */
_func_nfo 	*get_func_nfo_method_nfo(_method_nfo *); /* Weird, right? */
_class 		*get_class_method_nfo(_method_nfo *);
unsigned long long int get_flags_method_nfo(_method_nfo *); 

/* "Artificial" getters, from _func_nfo.h */
const char *get_name_method_nfo(_method_nfo *);
const char *get_ret_t_method_nfo(_method_nfo *);
const char **get_arg_type_method_nfo(_method_nfo *);
const char **get_args_method_nfo(_method_nfo *);
const unsigned get_arg_len_method_nfo(_method_nfo *);


#endif /* end of include guard: _METHOD_NFO_QL95EGPM */

