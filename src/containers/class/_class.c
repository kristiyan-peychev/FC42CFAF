#include "_class.h"
#include "../header.h"
#include <stdio.h>

_class *create_class(void) 
{ /* Default constructor, returns a pointer to an empty _class */
	_class *ret;
	ret = (_class *) malloc(1 * sizeof(_class));
	ret->name = NULL;
	ret->methods = NULL;
	ret->methods_len = 0;
	ret->flags = 0;
return ret;
}

void destroy_class(_class *target) 
{ /* Destructor, will free target */
	if (target == NULL) {
#ifdef FLG_VERBOSE
		fprintf(stderr, "YOU FUCKTARD, GIVING ME A NULL CLASS TO FREE\n");
#endif
		return;
	}
	if (target->name != NULL) 
		free(target->name);
	while (target->methods_len) 
		destroy_method_nfo(*(target->methods + --target->methods_len));
	if (target->methods != NULL) 
		free(target->methods);
	free(target);
}

_class *copy_class(_class *target) 
{ /* Similar to a copy-constructor, will not modify target */
	unsigned cc = 0;
	_class *ret;
	ret = (_class *) malloc(1 * sizeof(_class));
	if (target->name != NULL) {
	ret->name = (char *) malloc(strlen(target->name) * sizeof(char));
	strcpy(ret->name, target->name);
	} else {
		ret->name = NULL;
	}
	if (target->methods != NULL) {
		ret->methods = (_method_nfo **) malloc(target->methods_len * sizeof(_method_nfo *));
		while (cc < target->methods_len) {
			*(ret->methods + cc) = copy_method_nfo(*(target->methods + cc));
			cc++;
		}
	}
	ret->flags = target->flags;
	ret->methods_len = target->methods_len;
return ret;
}

void add_method_class(_class *target, _func_nfo *mtb, char flg) // MethodToBe
{ /* Add a method to the class target(1), with a function descriptor mtb(2) and flags(3), see _class.h */
	unsigned cc = 0;
	_method_nfo **mndptr;
	if (target->methods_len == 0) {
		target->methods = (_method_nfo **) malloc(1 * sizeof(_method_nfo *));
		*target->methods = create_method_nfo(target);
		set_method_nfo((*target->methods), mtb);
		target->methods_len = 1;
	} else {
		mndptr = target->methods;
		target->methods = (_method_nfo **) malloc((target->methods_len + 1) * sizeof(_method_nfo *));
		while (cc < target->methods_len) {
			*(target->methods + cc) = copy_method_nfo(*(mndptr + cc));
			cc++;
		}
		*(target->methods + cc) = create_method_nfo(target);
		set_method_nfo(*(target->methods + cc), mtb);
		target->methods_len++;
	}
	if (flg & METHOD_PURE_VIRTUAL)
		target->flags |= CLASS_PURE_ABSTRACT;
}

void set_name_class(_class *target, char *name, size_t name_sz)
{
	if (target->name != NULL) 
		free(target->name);
	target->name = (char *) malloc(name_sz * sizeof(char)); 
	strncpy(target->name, name, name_sz);
	target->name[name_sz] = '\0';
}

