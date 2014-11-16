#ifndef _CLASS_OOYVOHME

#define _CLASS_OOYVOHME

#define CLASS_PURE_ABSTRACT 1
#define CLASS_TEMPLATE 2

#include "header.h" 

typedef struct _class {
	char *name;
	_method_nfo **methods;
	unsigned methods_len;
	unsigned long long int flags;
} _class;

_class 	*create_class(void);
void 	destroy_class(_class *);
_class 	*copy_class(_class *);
void 	add_method_class(_class *, _func_nfo *, char);
void 	set_name_class(_class *, char *, size_t);

#endif /* end of include guard: _CLASS_OOYVOHME */

