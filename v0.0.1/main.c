#include <stdio.h>
#include "header.h"

int main(int argc, const char *argv[]) {
	int fd = open(*++argv, O_RDONLY);
	struct _test *test_p;
	char *dummy_main;
	if (search_for_special(fd)) {
		fprintf(stderr, "Couldn't find special character. Exiting\n");
		exit(1);
	}
	test_p = parse_command_string(fd);
	create_dummy_main(dummy_main, test_p);
	create_file_for_compiling(fd, dummy_main, SP_NEWFILE_NAME_AND_LOC);
	free(dummy_main);
	exec_gxx(SP_NEWFILE_NAME_AND_LOC, "");
	exec("./a.out");
return 0;
}

