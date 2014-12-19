#include "parser.h"

unsigned get_until_space(char *str, char **dest) 
{
	unsigned ret = 0;
	char *buf;

	while (*str != ' ' && *str != '\n' && *str != '\t' && *str != ')' && *str != '(' && *str != ',' && *str != ':') {
		++str;
		++ret;
	}

	buf = (char *) malloc((ret + 1) * sizeof(char));
	str -= ret;

	while (*str != ' ' && *str != '\n' && *str != '\t' && 
		*str != ')' && *str != '(' && *str != ',' && *str != ':') 

		*buf++ = *str++;

	*buf = '\0';
	buf -= ret;
	*dest = buf;
	return ret;
}

unsigned handle_args(_func_nfo **target, char *str)
{ // Untested
	unsigned n = 0, d = 0, rret = 0;
	_func_nfo *ret = *target; // Alias the target for uniform code
	char *namebuf, *typebuf, **bbuf;

	while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '(' || *str == ',')
		str++;

	while (*str != ',' && *str != ')') {
		n++;
		str++;
	}

	while (*str == ' ')
		str--;

	while (*str != ' ' && *str != '*' && *str != '&') {
		str--;
		n--;
	}

	typebuf = (char *) malloc ((n + 1) * sizeof(char));
	strncpy(typebuf, str - n, n);
	strcat(typebuf, "\0");

	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;

	bbuf = (char **) malloc(1 * sizeof(char *));
	d = get_until_space(str, bbuf);
	rret += n + d;
	namebuf = *bbuf;
	free(bbuf);
	str += d;
	add_arg_func_nfo(ret, typebuf, namebuf);

	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;

	if (*str != ')') {
		rret += handle_args(&ret, str);
	} 

	rret++;
return rret;
}

_func_nfo *parse_function(char **target)
{ /* Call this to parse the function and/or method you want to parse. There's a */
  /* more detailed description of what this does in the header */
	_func_nfo *ret;
	int n = 0, d = 0;
	char buf[10];
	char *str = *target, *namebuf, **bbuf;

	ret = create_func_nfo(); // Call default constructor

	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;

	while (*str != '(') {
		n++;
		str++;
	}

	while (*str == ' ')
		str--;

	while (*str != ' ' && *str != '*' && *str != '&') {
		str--;
		n--;
	}

	namebuf = (char *) malloc ((n + 2) * sizeof(char));
	strncpy(namebuf, str - n, (size_t) n+1);
	strcat(namebuf, "\0");
	set_ret_t_func_nfo(ret, namebuf, d + n + 1);
	free(namebuf);

	bbuf = (char **) malloc(1 * sizeof(char *)); // Reusing namebuf for the function's name
	d = get_until_space(str, bbuf);
	namebuf = *bbuf;
	free(bbuf);

	str += d;
	set_name_func_nfo(ret, namebuf, d);
	str += handle_args(&ret, str);

	#ifndef TEST_FUNC_NFO
	*target = str; // ACHTUNG!
	#endif
return ret;
}

/*
int main(int argc, const char *argv[]) {
	char buf[] = "asd ***&asd(asd asd)";
	char *buff = (char *) buf;
	_func_nfo *ret = parse_function(&buff);
	fprintf(stderr, "%s %s (%s %s) f?\n", ret->ret_t, ret->name, *(ret->arg_type), *(ret->args));
	char test2[] = "FU_f_-CK *_GH_0as       \t(aaa a asd, a *asd) ";
	buff = (char *) test2;
	ret = parse_function(&buff);
	fprintf(stderr, "%s %s (%s %s) %d\n", ret->ret_t, ret->name, *(ret->arg_type), *(ret->args), ret->arg_len);
return 0;
}
*/

