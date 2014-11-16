#ifndef _STRUCT_H_
#define _STRUCT_H_

#define SP_TEST_CHAR '?'
#define SP_UNIQ_CHAR "^ut_@"

#define FLG_TEST_RET 00
#define FLG_TEST_ARG ~FLG_TEST_RET
#define FLG_INCL_LOCAL 1

#define FLG_INCL_CXX 2

struct _func_nfo {
	char *name;
	char **arg_type;
	char **args;
	int arg_len;
	unsigned long long int flags;
};

struct _incl {
	char *name;
	int flags;
};

int parse_command_string(int);
int create_dummy_main(char *, struct _func_nfo *);
struct _func_nfo *parse_function(int, char[], struct _incl **, int *);
void destroy_incl(struct _incl *);
void destroy_func_nfo(struct _func_nfo *);
int *build_bad_Match_table(char *);


/* struct {
	char _int[] = "int";
	char _short[] = "short";
	char _long[] = "long";
	char _char[] = "char";
	char _bool[] = "bool";
	char _float[] = "float";
	char _dfloat[] = "double";
} _default_types;

char **_default_types_array[] = {{(char *) _default_types}, {(char *) _default_types + 3}, {(char *) _default_types + 8}, {(char *) _default_types + 12}, {(char *) _default_types + 16}, {(char *) _default_types + 20}, {(char *) _default_types + 25}};*/

#endif

