#include "parser.h"

#define TOKEN_PUBLIC "public" /* 6 */
#define TOKEN_PRIVATE "private" /* 7 */
#define TOKEN_PROTECTED "protected" /* 9 */

#define ACTIVE_PRIVATE 1
#define ACTIVE_PROTECTED 2
#define ACTIVE_PUBLIC 4

char is_access_mod(char *str)
{ /* FIXME better way to do this? */
	unsigned f = strlen(str);
	if (f != 6 && f != 7 && f != 9) /* The length of each access token as listed above */
		return 0;
	if (f == 6) {
		if (!strncmp(TOKEN_PUBLIC, str, 6))
			return ACTIVE_PUBLIC;
	} else if (f == 7) {
		if (!strncmp(TOKEN_PRIVATE, str, 7))
			return ACTIVE_PRIVATE;
	} else if (f == 9) {
		if (!strncmp(TOKEN_PROTECTED, str, 9))
			return ACTIVE_PROTECTED;
	} else {
		return 0; /* Error out, syntax error or something else? */
	}
}

unsigned check_for_afterdescriptors(char *str, char *pflagbuf) 
{ /* const at the end of a method, afterdescriptor, lel */
	unsigned ret = 0, n = 0;
	char *chkbuf;
	while (*str == ' ') {
		++ret;
		++str;
	}
	while (*str != ';' && *str != '{') { 
		if (*str == ' ') {
			++str;
			continue;
		}
		++str;
		++n;
	}
	str -= n;
	chkbuf = (char *) malloc((n + 1) * sizeof(char *)); 
	strncpy(str, chkbuf, n);
	chkbuf[n] = '\0';
	if (!strcmp(chkbuf, "const")) 
		*pflagbuf |= METHOD_CONST_MOD;
	free(chkbuf);
	ret += n;
	return ret;
}

unsigned parse_class_body(_class *target, char **target) 
{
	unsigned active_mode = ACTIVE_PRIVATE; /* In C++ the default access modifier is `private` */ /* FIXME: choose default */
	unsigned ret, d = 0, n = 0;
	char *str = *target, somebuf, flagbuf = 0;
	char **bbuf = (char **) malloc(1 * sizeof(char *));
	_func_nfo *fbuf;
	while (1) {
		d = get_until_space(str, bbuf);
		somebuf = is_access_mod(str);
		str += d;
		if (!somebuf) {
		} else {
			active_mode = somebuf;
		}
		*bbuf = str;
		fbuf = parse_function(bbuf);
		free(*bbuf);
		while (*str == ' ') 
			str++;
		if (*str == '=') { /* FIXME */
			while (*str == ' ') 
				str++;
			if (*str == '0') 
				flagbuf |= METHOD_PURE_VIRTUAL;
		} 
		d = check_for_afterdescriptors(str, &flagbuf); 
		add_method_class(target, fbuf, (active_mode >> 3) | flagbuf); /* Converting to the _method_nfo.h file flags FIXME */
		destroy_func_nfo(fbuf);
	}
	free(bbuf);
	return ret;
}

// TODO: inheritance
_class *parse_class(char **target)
{
	char *str = *target, **bbuf, *namebuf;
	unsigned n = 0, d = 0;
	_class *ret;
	ret = create_class();
	while (*str == ' ' || *str == '\t' || *str == '\n') 
		str++; /* Skip whitespaces at the front */

	if (*str == ';') {
		#ifdef FLG_VERBOSE  /* FIXME and make it uniform */
		fprintf(stderr, "Class declaration, crap.\n");
		#endif 
		destroy_class(ret); 	/* There is no point in having a class in name only */
		return NULL;  		/* so I leave it for when we see its declaration */
	}

	bbuf = (char **) malloc(1 * sizeof(char *)); /* Needed for `get_until_space` */
	d = get_until_space(str, bbuf);
	namebuf = *bbuf;
	free(bbuf);
	set_name_class(ret, namebuf, d); 
	free(namebuf);
	while (*str == ' ' || *str == '\t' || *str == '\n')
		++str;
	if (*str == ':') { /* INHERITANCE! TODO */
	} else if (*str == '{') { /* Definition, or hopefully a declaration, of a class */
		d = parse_class_body(ret, &str);
	} else if (*str == ';') { /* Declaration is somewhere else, no need for this to exist, returning nil */
		destroy_class(ret);
		return NULL;
	}
	return ret;
}

/* TODO TEST THIS SHIT */
