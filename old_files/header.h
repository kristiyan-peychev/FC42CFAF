#ifndef _STRUCT_H_
#define _STRUCT_H_

/* character and string constants */
#define SP_TEST_CHAR '?'
#define SP_UNIQ_CHAR "^ut_@"
#define SP_NEWFILE_NAME_AND_LOC "/home/kawaguchi/.UTest/fuck.cpp"
#define SP_NEWEXEC_NAME_AND_LOC "/home/kawaguchi/.UTest/fuck"
#define SP_GXX_NAME "g++"

/* include flags */
#define FLG_TEST_RET 1
#define FLG_TEST_ARG(x) 1<<x

#define FLG_INCL_LOCAL 1
#define FLG_INCL_CXX 2

/* data containers */
struct _func_nfo {
	char *name; // function name
	char *ret_t; // function return type
	char **arg_type; // data types of the function's arguments
	char **args; // function's arguments
	int arg_len; // number of arguments
	unsigned long long int flags; // guess
};

#ifdef _IM_V2_
	struct _test {
		struct _func_nfo *func; // function to test description
		char ***test_value; // function to test value
		int *test_value_sz; // whatever
		int test_cnt; // number of tests
		unsigned long long int *flags; // which arguments to test
	};
#else 
	struct _test {
		struct _func_nfo *func; // function to test description
		char **test_value; // function to test value
		int test_value_sz; // number of tests?
		unsigned long long int flags; // which arguments to test
	};
#endif

struct _incl { /* unused */
	char *name;
	int flags;
};

/* core.c: */
#ifdef UT_IM
	struct _test **parse_command_string(int, size_t *);
	int create_dummy_main(char **, struct _test **, size_t);
#else
	struct _test *parse_command_string(int);
	int create_dummy_main(char **, struct _test *);
#endif
	struct _func_nfo *parse_function(int, char[], struct _incl **, int *);
	void destroy_incl(struct _incl *);
	void destroy_func_nfo(struct _func_nfo *);
	void destroy_test(struct _test *);
	char **substr(char *, char *); /* Bess */
	int search_for_special(int); /* Auto called by create_file_for compiling */
	int create_file_for_compiling(int, char *, char *); /* File descriptor + new main function + file name */

/* exec.c: */
	int exec(char *); /* returns exit value of child process */
	int exec_gxx(char *, char *); /* File-to-compile name + params fr g++; returns 0 */
#endif

