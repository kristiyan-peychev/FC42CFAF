#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//#include "header.h"

// ^ut_@?4(?2,2)
int fun2test(int *i, int f)
{
	*i = f;
	return *i + f;
}

// ^ut_@?2(2)
int fuck_(int i)
{
	return i;
}

// ^ut_@?3(1,1,1)
int kurac(int f, int s, int gg)
{
	return f + s + gg;
}

// ^ut_@?4(2)
int tashaci(int ff)
{
	return ff;
}

/*void init_test(char *path)
{
	int fd = open(path, O_RDONLY);
	struct _test *fun2test;
	if ((fun2test = parse_command_string(fd)) == NULL){
		destroy_test(fun2test);
		exit(1);
	}
}
*/
