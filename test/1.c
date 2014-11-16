#include <stdio.h>

struct fawk {
	int one;
	int two;
	int (*fp1)(void);
	int (*fp2)(void);
};

int fun1(void) 
{
	return 1;
}

int fun2(void)
{
	return 2;
}

int main(int argc, const char *argv[]) {
	struct fawk f = {1, 2, fun1, fun2};
	int n = f.fp1();
return 0;
}
