#ifndef _TEMPLATE_49FXXB1X

#define _TEMPLATE_49FXXB1X

#include "../header.h"

#define TEMPLATE_CLASS 1
#define TEMPLATE_FUNC 2

union _template_u {
	_class *clp;
	_func_nfo *fnp;
};

typedef struct _template {
	unsigned long long int flags;
	union _template_u p;
} _template;

_template *create_template(unsigned long long int);
void destroy_template(_template *);
void change_template_pointer_to_class(_template *, _class *);
#define chptoclass(x,y) change_template_pointer_to_class(x, y) 
void change_template_pointer_to_func(_template *, _func_nfo *);
#define chptofunc(x, y) change_template_pointer_to_func(x, y) 
unsigned long long int get_type_template(_template *); 
void *get_template(_template *); 

#endif /* end of include guard: _TEMPLATE_49FXXB1X */

