#ifndef SUBSTR_D81FGDW0

#define SUBSTR_D81FGDW0

int *build_bad_match_table(char *sub); /* Ret value has to be freed after */
				       /* being used, but the algorithm does */
				       /* that by itself, so no need to concern */
				       /* yourself with it */
char **substr(char *str, char *sub); /* This will return you pointers to the beginning */
				     /* of every match it finds, and is terminated by  */
				     /* a nil pointer */

#endif /* end of include guard: SUBSTR_D81FGDW0 */
