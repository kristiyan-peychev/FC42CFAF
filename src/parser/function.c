#include "parser.h"

/* The functions from here on down are supposedly tested and work correctly to some extent.
 * They do need more extensive testing at some extreme scenarios. TODO */

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
	while (*str != ' ' && *str != '\n' && *str != '\t' && *str != ')' && *str != '(' && *str != ',' && *str != ':') /* Note? */
		*buf++ = *str++;
	*buf = '\0';
	buf -= ret;
	*dest = buf;
return ret;
}

static unsigned handle_args(_func_nfo **target, char *str)
{ /* I think this might need more testing */
	unsigned n = 0, d = 0, rret = 0; /* Real return */
	_func_nfo *ret = *target; /* Alias the target for uniform code */
	char *namebuf, *typebuf, **bbuf;
	while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '(' || *str == ',')
		str++;
	if (*str == ')') {
		return 0;
	}
	/* Obsolete. TODO: remove
	bbuf = (char **) malloc(1 * sizeof(char *));
	d = get_until_space(str, bbuf);
	typebuf = *bbuf;
	free(bbuf);
	str += d;
	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;
	while (*str == '*' || *str == '&') {
		str++;
		n++;
	}
	typebuf = (char *) realloc(typebuf, (d + n + 1) * sizeof(char *));
	str -= n;
	*/
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
	bbuf = (char **) malloc(1 * sizeof(char *));
	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;
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
		str++; /* Skip whitespace characters at the beginning of the string */

	/* Obsolete  TODO remove
	bbuf = (char **) malloc(1 * sizeof(char *));
	d = get_until_space(str, bbuf);
	namebuf = *bbuf;
	free(bbuf);
	str += d;
	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;
	while (*str == '*' || *str == '&') {
		str++;
		n++;
	}
	namebuf = (char *) realloc(namebuf, (d + n + 1) * sizeof(char *));
	str -= n;
	n = 0;
	while (*str == '*' || *str == '&') 
		namebuf[d + n++] = *str++;
	namebuf[d + n] = '\0';
	set_ret_t_func_nfo(ret, namebuf, d + n + 1);
	free(namebuf);
	*/

	while (*str != '(') {
		++n;
		++str;
	}

	do {
		--str;
		--n;
	} while (*str == ' ' || *str == '\t' || *str == '\n');

	while (*str != ' ' && *str != '*' && *str != '&') {
		--str;
		--n;
	}

	++str;
	++n;

	namebuf = (char *) malloc ((n + 1) * sizeof(char));
	strncpy(namebuf, str - n, (size_t) n);
	strcat(namebuf, "\0");
	set_ret_t_func_nfo(ret, namebuf, d + n);
	free(namebuf);
	bbuf = (char **) malloc(1 * sizeof(char *)); /* Reusing namebuf for the function's name to store */
	d = get_until_space(str, bbuf);
	namebuf = *bbuf; /* Save the real return value from `get_until_space` */
	free(bbuf); 
	str += d;
	set_name_func_nfo(ret, namebuf, d);
	free(namebuf);
	str += handle_args(&ret, str);
	#ifndef TEST_FUNC_NFO
	*target = str; /* ACHTUNG! */
	#endif
return ret;
}

#if 0
int main(int argc, const char *argv[]) {
	char buf[] = "asd ***&asd(asd asd)";
	char *buff = (char *) buf;
	_func_nfo *ret = parse_function(&buff);
	fprintf(stderr, "%s %s (%s %s) f?\n", ret->ret_t, ret->name, *(ret->arg_type), *(ret->args));
	destroy_func_nfo(ret);
	char test2[] = "asd ffdf()";
	buff = (char *) test2;
	ret = parse_function(&buff);
	fprintf(stderr, "Ret_t: `%s` name: `%s` (%s %s) %d\n", ret->ret_t, ret->name, *(ret->arg_type), *(ret->args), ret->arg_len);
	fprintf(stderr, "Name: `%s'\n", ret->name);
	destroy_func_nfo(ret);
return 0;
}
#endif
