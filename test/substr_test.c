#include "../containers/substr.h"
#include <stdio.h>

static char bigbuff[100000000];

int main(int argc, const char *argv[]) {
	fgets(bigbuff, 100000000, stdin);
	substr(bigbuff, "4");
return 0;
}
