#include "../header.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

_func_nfo *create_func_nfo(void) 
{ /* Default contructor,  returns a pointer to an empty _func_nfo */
	_func_nfo *ret;
	ret = (_func_nfo *) malloc(1 * sizeof(_func_nfo));
	memset(ret, 0, sizeof(_func_nfo)); /* This does the same things as below */
	/*
	ret->name = ret->ret_t = NULL;
	ret->arg_type = ret->args = NULL;
	ret->arg_len = 0;
	ret->flags = 0; // FIXME?
	*/
return ret;
}

void destroy_func_nfo(_func_nfo *ret) 
{ /* Destructor, will free the argument you pass it */
	if (ret->name != NULL) 
		free(ret->name);
	if (ret->ret_t != NULL)
		free(ret->ret_t);
	while (ret->arg_len > 0) {
		if (ret->arg_type != NULL && *(ret->arg_type + ret->arg_len - 1) != NULL) 
			free(*(ret->arg_type + ret->arg_len - 1));
		if (ret->args != NULL && *(ret->args + ret->arg_len - 1) != NULL) 
			free(*(ret->args + ret->arg_len - 1));
		ret->arg_len--;
	}
	if (ret->arg_type != NULL) 
		free(ret->arg_type);
	if (ret->args != NULL)
		free(ret->args);
	free(ret);
}

_func_nfo *copy_func_nfo(_func_nfo *target) 
{ /* Similar to a copy-constructor, will clone the target and return a new identical one, target is not modified */
	unsigned cc;
	_func_nfo *ret;
	ret = create_func_nfo();
	if (target == NULL || (target->name == NULL && target->ret_t == NULL && target->arg_type == NULL && target->args == NULL))
		return ret;
	if (target->name != NULL) {
		ret->name = (char *) malloc(strlen(target->name) * sizeof(char));
		strcpy(ret->name, target->name);
	} else {
		ret->name = NULL;
	}
	if (target->ret_t != NULL) {
		ret->ret_t = (char *) malloc(strlen(target->ret_t) * sizeof(char));
		strcpy(ret->ret_t, target->ret_t);
	}
	else {
		ret->ret_t = NULL;
	}
	if (target->arg_type != NULL) {
		ret->arg_type = (char **) malloc(target->arg_len * sizeof(char *));
		while (cc < target->arg_len) {
			*(ret->arg_type + cc) = (char *) malloc(strlen(*(target->arg_type + cc)) * sizeof(char));
			strcpy(*(ret->arg_type + cc), *(target->arg_type + cc));
			cc++;
		}
	} else {
		ret->arg_type = NULL;
	}
	if (target->args != NULL) {
		ret->args = (char **) malloc(target->arg_len * sizeof(char *));
		cc = 0;
		while (cc < target->arg_len) {
			*(ret->args + cc) = (char *) malloc(strlen(*(target->args + cc)) * sizeof(char));
			strcpy(*(ret->args + cc), *(target->args + cc));
			cc++;
		}
	} else {
		ret->args = NULL;
	}
	ret->arg_len = target->arg_len;
	ret->flags = target->flags;
return ret;
}

/* Mutators */
void set_name_func_nfo(_func_nfo *target, const char *name, unsigned n) 
{
	if (target->name != NULL)
		free(target->name);
	target->name = (char *) malloc(n * sizeof(char) + 1 * sizeof(char));
	strncpy(target->name, name, n);
	target->name[n] = '\0';
}

void set_ret_t_func_nfo(_func_nfo *target, const char *ret_type, unsigned n) 
{
	if (target->ret_t != NULL) 
		free(target->ret_t);
	target->ret_t = (char *) malloc(n * sizeof(char) + 1 * sizeof(char));
	strncpy(target->ret_t, ret_type, n);
	target->ret_t[n] = '\0';
}

void add_arg_func_nfo(_func_nfo *target, const char *arg_t, const char *arg) 
{
	unsigned cc = 0;
	char **dctmp0, **dctmp1;
	if (arg_t == NULL && arg == NULL)
		return;
	if (target->arg_len == 0) {
		if (arg_t != NULL) {
			target->arg_type = (char **) malloc(1 * sizeof(char *));
			*target->arg_type = (char *) malloc(strlen(arg_t) * sizeof(char));
			strcpy(*target->arg_type, arg_t);
			strcat(*target->arg_type, "\0");
		}
		if (arg != NULL) {
			target->args = (char **) malloc(1 * sizeof(char *));
			*target->args = (char *) malloc(strlen(arg) * sizeof(char));
			strcpy(*target->args, arg);
			strcat(*target->args, "\0");
		}
	} else {
		if (arg_t != NULL) {
			dctmp0 = target->arg_type;
			target->arg_type = (char **) malloc((target->arg_len + 1) * sizeof(char *));
		}
		if (arg != NULL) {
			dctmp1 = target->args;
			target->args = (char **) malloc((target->arg_len + 1) * sizeof(char *));
		}
		cc = 0;
		while (cc < target->arg_len) {
			if (arg_t != NULL) {
				*(target->arg_type + cc) = (char *) malloc(strlen(*(dctmp0 + cc)) * sizeof(char));
				strcpy(*(target->arg_type + cc), *(dctmp0 + cc));
				strcat(*(target->arg_type + cc), "\0");
			}
			if (arg != NULL) {
				*(target->args + cc) = (char *) malloc(strlen(*(dctmp1 + cc)) * sizeof(char));
				strcpy(*(target->args + cc), *(dctmp1 + cc));
				strcat(*(target->args + cc), "\0");
			}
			cc++;
		}
	}
	target->arg_len++;
}

/* Getters */

inline const char *get_name_func_nfo(_func_nfo *target) 
{
	return target->name;
}

inline const char *get_ret_t_func_nfo(_func_nfo *target)
{
	return target->ret_t;
}

inline const char **get_arg_type_func_nfo(_func_nfo *target) 
{
	return (const char **) target->arg_type;
}

inline const char **get_args_func_nfo(_func_nfo *target)
{
	return (const char **) target->args;
}

inline const unsigned get_arg_len_func_nfo(_func_nfo *target) 
{
	return (const unsigned) target->arg_len;
}

inline const long long int get_flags_func_nfo(_func_nfo *target)
{
	return (const long long int) target->flags;
}

/*
int main(int argc, const char *argv[]) {
	unsigned i = 0;
	_func_nfo *test;
	test = create_func_nfo();
	set_name_func_nfo(test, "FUCK", 4u);
	set_ret_t_func_nfo(test, "FAWK", 4u);
	while (i < 0x1421ul) {
		add_arg_func_nfo(test, "ASD", "FGH");
		i++;
	}
return 0;
}
*/

