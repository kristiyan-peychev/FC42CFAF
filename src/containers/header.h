#ifndef HEADER_GX2ZZ528

#define HEADER_GX2ZZ528

#include "substr.h"

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

/* containers */
typedef struct _class _class;
typedef struct _method_nfo _method_nfo;
typedef struct _func_nfo _func_nfo;
typedef struct _template _template;

#include "_class.h"
#include "_method_nfo.h"
#include "_func_nfo.h"
#include "_template.h"

/* exec.c: */
	int exec(char *); /* returns exit value of child process */
	int exec_gxx(char *, char *); /* File-to-compile name + params fr g++; returns 0 */

#endif /* end of include guard: HEADER_GX2ZZ528 */
