#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>

#include "header.h"
#include "substr.h"

int *build_bad_match_table(char *sub)
{ /* You will need to free(table) after you've found (or not) the substring */
	int i = 0, len = strlen(sub);
	int *table;
	table = (int *) malloc(128 * sizeof(int));
	if (table == NULL) {
		fprintf(stderr, "ERROR: Not enough memory\n");
		exit(0);
	}
	bzero(table, 128 * sizeof(int));
	while (i < len - 1) {
		*(table + *(sub + i)) = len - i - 1;
		i++;
	}
return table;
}

char **substr(char *big, char *sub)
{ /* Boyer-Moore-Horspool substring algorithm, modified a bit, and it might well work really fast... */
	int i = 0, len = strlen(sub), blen = strlen(big), offset = len - 1; /* FIXME strlen */
	int *table = build_bad_match_table(sub), f = 0, occurances = 0;
	char **ret; /* I will return this */

	ret = (char **) malloc((blen / len) * sizeof(char *)); /* Remember to free this later, kids! */
	if (ret == NULL) {
		fprintf(stderr, "Substring ERROR: Not enough memory\n");
		exit(0);
	}

	bzero(ret, (blen / len) * sizeof(char *));
	/*i = 0;*/
	do {
		
		if (i > blen) 
			break;
		
		do {
			/*fprintf(stderr, "TEST %c, %c; i = %d; offset = %d\n", *(big + i + offset), *(sub + offset), i, offset);*/
			if (offset <= 0) {
				*(ret + f) = big + i;
				occurances++;
				i++;
				f++;
				break;
			} else if (i + offset >= blen) {
				i++;
				break;
			}
			if (*(big + i + offset) == *(sub + offset)) {
				offset--;
			} else {
				/*fprintf(stderr, "i += %d\n", *(table + *(big + i + offset)) ? *(table + *(big + i + offset)) : len);*/
				i += (*(table + *(big + i + offset)) ? *(table + *(big + i + offset)) : len);
				break;
			}
		} while (1);

		offset = len - 1;

	} while (1);

	free(table);
	/*fprintf(stderr, "SUBSTR RET: %d\n", occurances);*/
	if (occurances == 0) {
		free(ret);
		return NULL;
	}
return ret;
}

