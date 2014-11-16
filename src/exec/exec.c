#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "header.h"

char **split_charstar(char *star)
{
	int i = 0, j = 1, d = 0, ij;
	char **ret;
	while (*(star + i)) {
		if (*(star + i) == ' ') {
			j++;
		}
		i++;
	}
	ret = (char **) malloc(j * sizeof(char *));
	ij = ++i;
	i = 0;
	while (i < j) {
		*(ret + i) = (char *) malloc(ij * sizeof(char));
		i++;
	}
	i = j = 0;
	while (d < ij) {
		if (*(star + d) == ' ') {
			*(*(ret + j) + i) = '\0';
			j++;
			i = 0;
			d++;
			continue;
		}
		*(*(ret + j) + i) = *(star + d);
		i++;
		d++;
	}
	*(ret + ++j) = (char *) 0;
return ret;
}

void rlexec(char *string)
{
	char **mbuf = split_charstar(string);
	int  i = 0;
	if (execvp(*mbuf, mbuf) < 0) {
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	while (*mbuf != (char *) 0)
		free(*mbuf++);
	free(mbuf);
}

int exec(char *string)
{
	int status;
	pid_t cpid, w;
	cpid = fork();
	if (cpid == 0) {
		fprintf(stderr, "Executing `%s`\n", string);
		rlexec(string);
	} else {
		w = waitpid(cpid, &status, WUNTRACED);
		if (w == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		return WEXITSTATUS(status);
	}
return 0;
}

int exec_gxx(char *file_name, char *additional_params)
{ 
	char *exec_string, buf;
	int i = 0, lenbuf;
	lenbuf = strlen(file_name) + strlen(additional_params) + strlen("g++ -o ") + 64;
	exec_string = (char *) malloc((lenbuf + 1) * sizeof(char));
	sprintf(exec_string, "%s %s -o %s", SP_GXX_NAME, file_name, SP_NEWEXEC_NAME_AND_LOC);
	if ((i = exec(exec_string))){
		//free(exec_string);
		exit(0);
	} else {
		fprintf(stderr, "Child process exited with %d\n", i);
	}
	free(exec_string);
return 0;
}

