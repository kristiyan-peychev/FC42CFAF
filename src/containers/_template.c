#include "_template.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_template *create_template(unsigned long long int flags) 
{
	_template *ret;
	ret = (_template *) malloc(1 * sizeof(_template));
	ret->flags = flags;
	memset(&ret->p, 0, sizeof(union _template_u));
return ret;
}

void destroy_template(_template *target)
{
	if (target->flags & TEMPLATE_CLASS) {
		free(target->p.clp);
	} else if (target->flags & TEMPLATE_FUNCT) {
		free(target->p.clp);
	} else {
		// Error
		fprintf(stderr, "Error: Wrong flags passed to `destroy_template`\n");
		return;
	}
}

void change_template_pointer_to_class(_template *target, _class *to)
{
	destroy_template(target);
	target = create_template(TEMPLATE_CLASS);
	target->p.clp = to;
}

void change_template_pointer_to_func(_template *target, _func_nfo *to)
{
	destroy_template(target);
	target = create_template(TEMPLATE_FUNC);
	target->p.fnp = to;
}

unsigned long long int get_type_template(_template *target) 
{
	return target->flags;
}

void *get_template(_template *target) 
{
	if (target->flags == TEMPLATE_CLASS) {
		return (void *) template->p.clp;
	} else if (target->flags == TEMPLATE_FUNC) {
		return (void *) template->p.fnp;
	}
}

