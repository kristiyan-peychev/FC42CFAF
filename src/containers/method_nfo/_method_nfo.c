#include "../header.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

_method_nfo *create_method_nfo(_class *super)
{ /* Default constructor, will return a pointer to an empty _method_nfo */
	_method_nfo *ret;
	ret = (_method_nfo *) malloc(1 * sizeof(_method_nfo));
	ret->func = create_func_nfo();
	ret->super = super;
	ret->flags = 0;
return ret;
}

void destroy_method_nfo(_method_nfo *target) 
{ /* Destructor, will free its target */
	destroy_func_nfo(target->func);
	free(target);
}

_method_nfo *copy_method_nfo(_method_nfo *target) 
{ /* Similar to a copy-constructor, does not modify target */
	_method_nfo *ret;
	ret = (_method_nfo *) malloc(1 * sizeof(_method_nfo)); 
	ret->func = copy_func_nfo(target->func);
	ret->super = target->super;
	ret->flags = target->flags;
return ret;
}

/* Setters */
void set_method_nfo(_method_nfo *target, _func_nfo *ff) 
{
	if (target->func != NULL) {
		destroy_func_nfo(target->func);
		free(target->func);
	}
	target->func = copy_func_nfo(ff);
}

inline void set_super_method_nfo(_method_nfo *target, _class *super) 
{
	target->super = super;
}

inline _func_nfo *get_func_nfo_method_nfo(_method_nfo *target) 
{ 
	return target->func;
}

inline _class *get_class_method_nfo(_method_nfo *target) 
{
	return target->super;
}

inline unsigned long long int get_flags_method_nfo(_method_nfo *target) 
{
	return target->flags;
}

inline const char *get_name_method_nfo(_method_nfo *target) 
{
	return get_name_func_nfo(get_func_nfo_method_nfo(target)); /* lulz */
}

inline const char *get_ret_t_method_nfo(_method_nfo *target) 
{
	return get_ret_t_func_nfo(get_func_nfo_method_nfo(target));
}

inline const char **get_arg_type_method_nfo(_method_nfo *target) 
{
	return get_arg_type_func_nfo(get_func_nfo_method_nfo(target));
}

inline const char **get_args_method_nfo(_method_nfo *target)
{
	return (const char **) get_arg_type_func_nfo(target->func);
}

inline const unsigned get_arg_len_method_nfo(_method_nfo *target) 
{
	return (const unsigned) get_arg_len_func_nfo(target->func);
}

/*
int main(int argc, const char *argv[]) {
	_class *class;
	_method_nfo *test;
	_func_nfo *ff;
	test = create_method_nfo(class);
	class = create_class();
	ff = create_func_nfo();
	set_name_func_nfo(ff, "asd", 3);
	set_method_nfo(test, ff);
	set_method_nfo(test, ff);
	add_method_class(class, ff, 0);
	add_method_class(class, ff, 0);
return 0;
}
*/

