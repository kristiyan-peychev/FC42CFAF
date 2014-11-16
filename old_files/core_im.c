#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>

#define UT_IM
#include "header.h"



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
{ /* Boyer-Moore-Horspool substring algorithm, modified a bit */
	int i = 0, len = strlen(sub), blen = strlen(big), offset = len - 1;
	int *table = build_bad_match_table(sub), f = 0, occurances = 0;
	char **ret;
	ret = (char **) malloc((blen / len) * sizeof(char *));
	if (ret == NULL) {
		fprintf(stderr, "ERROR: Not enough memory\n");
		exit(0);
	}
	bzero(ret, (blen / len) * sizeof(char *));
	i = 0;
	while (1) {
		if (i > blen) {
			break;
		}
		while (1) {
			//fprintf(stderr, "TEST %c, %c; i = %d; offset = %d\n", *(big + i + offset), *(sub + offset), i, offset);
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
				//fprintf(stderr, "i += %d\n", *(table + *(big + i + offset)) ? *(table + *(big + i + offset)) : len);
				i += (*(table + *(big + i + offset)) ? *(table + *(big + i + offset)) : len);
				break;
			}
		}
		offset = len - 1;
	}
	free(table);
	//fprintf(stderr, "SUBSTR RET: %d\n", occurances);
	if (occurances == 0) {
		free(ret);
		return NULL;
	}
return ret;
}

void destroy_func_nfo(struct _func_nfo *str)
{
	while (--str->arg_len) {
		free(*(str->args + str->arg_len));
		free(*(str->arg_type + str->arg_len));
	}
	free(str->ret_t);
	free(str->name);
	free(str);
}

void destroy_incl(struct _incl *str)
{
	free(str->name);
	free(str);
}

void destroy_test(struct _test *str)
{
	int i = 0;
	destroy_func_nfo(str->func);
	while (i < str->test_value_sz) {
		free(*(str->test_value + i));
		i++;
	}
	free(str->test_value);
	free(str);
}

int alloc_double(char *target, int *size)
{
	*size *= 2;
	target = (char *) realloc(target, *size * sizeof(void *));
	if (target == NULL) {
		return 1;
	} else {
		return 0;
	}
}

struct _incl *identify_include(char *str)
{ /* Pass one line of code to the function, remove the hashtag in front */
	int i = 0;
	struct _incl *incld;
	char chck[8], buf[256] = {};
	while (*str == ' ') 
		str++;
	strncpy(chck, str, 7);
	chck[8] = '\0';
	if (!strcmp(chck, "include")) 
		return NULL;
	incld = (struct _incl *) malloc(1 * sizeof(struct _incl));
	if (incld == NULL) {
		fprintf(stderr, "ERROR: Not enough memory\n");
		exit(0);
	}
	while (*str != '<' || *str != '"') {
		if (*str == '\0') {
			free(incld);
			return NULL;
		}
		++str;
	}
	while (*(str + i) != '>' || *(str + i) != '"') {
		if (*(str + i) == '\0') {
			free(incld);
			return NULL;
		}
		*(buf + i) = *(str + i);
		i++;
	}
	incld->name = (char *) malloc(i * sizeof(char));
	sprintf(incld->name, "%s%c", buf, 0);
	if ((*(str - 1) == '<' && *(str + i + 1) == '"') || (*(str - 1) == '>' && *(str + i + 1) == '"')) {
		free(incld->name);
		free(incld);
		return NULL;
	} else {
		if (*(str - 1) == '<') {
			incld->flags = FLG_INCL_CXX;
		} else {
			incld->flags = FLG_INCL_LOCAL;
		}
	}
return incld;
}

int copy_reverse(char *dest, char *src)
{
	int s = strlen(src) - 1, d = 0;
	while (s >= 0) {
		dest[d] = src[s];
		--s;
		++d;
	}
	dest[d] = '\0';
return d;
}

void split_type_from_name(char *src, char **type, char **name)
{
	int s = 0, d = 0;
	char buff[256] = "";
	while (*src == ' ')
		src++;
	while (*src != ' ') {
		d[buff] = *src;
		d++;
		src++;
	}
	d[buff] = '\0';
	*type = (char *) malloc((d + 1) * sizeof(char));
	sprintf(*type, "%s%c", buff, '\0');
	bzero(buff, 0);
	s = d;
	while (*src && *src != ' ') {
		d[buff] = *src;
		d++;
		src++;
	}
	d[buff] = '\0';
	*name = (char *) malloc((d - s + 1) * sizeof(char));
	sprintf(*name, "%s%c", buff, '\0');
}

/*void split_type_from_name(char *src, char **type, char **name)
{
	int s = 0, d = 0, src_len = strlen(src)-1, s_len;
	char buff[256];
	while (src[src_len] == ' ') 
		--src_len;
	while (src_len) {
		if (src[src_len] == ' ') {
			--src_len;
			break;
		}
		buff[s] = src[src_len];
		--src_len;
		++s;
	}
	buff[s] = '\0';
	*name = (char *) malloc((s + 1) * sizeof(char));
	*type = (char *) malloc((src_len + 1) * sizeof(char));
	copy_reverse(*name, buff);
	//bzero(buff, 256);
	s_len = src_len;
	while (src_len) {
		fprintf(stderr, "wtf %c(%d)\n", src[s_len - src_len], src[s_len - src_len]);
		*((*type) + d) = src[s_len - src_len];
		++d;
		--src_len;
	}
	while (*((*type) + s_len) == ' ') {
		*((*type) + s_len) = '\0';
		--s_len;
	}
	fprintf(stderr, "AA? `%s` %s\n", *type, *name);
}*/

struct _func_nfo *parse_function(int fd, char f_name[], struct _incl **inc, int *inc_size)
{
	int i = 0, f = 0, flg = 0, s = 0;
	int name_len = strlen(f_name);
	char buff2[5000]; // CHANGE THIS SHIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!1
	char buff1[5000], ff, l = 0;
	struct _incl **inclds = NULL;
	struct _func_nfo *fun = (struct _func_nfo *) malloc(1 * sizeof(struct _func_nfo));
	while (read(fd, buff2 + i, 1) > 0) {
		if (i > name_len) {
			*(buff2 + i + 1) = '\0';
			if (substr(buff2, f_name)) {
				break;
			}
		} else if (*(buff2 + i) == '#' && flg ^ 1) {
			flg |= 1;
		} else if (flg & 1 && *(buff2 + i) != '\n') {
			*(buff1 + l) = *(buff2 + i);
			l++;
		} else if (flg & 1 && *(buff2 + i) == '\n') {
			s++;
			inclds = (struct _incl **) realloc(inclds, s * sizeof(struct _incl *));
			*(inclds + s - 1) = identify_include(buff1);
			l = 0;
		}
		i++;
	}
	inc = inclds;
	*inc_size = s;
	i = 0;
	while (read(fd, &ff, 1) > 0) {
		if (ff == '(') {
			continue;
		} else if (ff == ')') {
			*(buff1 + i) = ff;
			i++;
			*(buff1 + i) = '\0';
			break;
		} else {
			*(buff1 + i) = ff;
			i++;
		}
	}
	i = 0;
	fun->arg_len = 1;
	while (*(buff1 + i)) {
		if (*(buff1 + i) == ',') {
			fun->arg_len++;
		} 
		i++;
	}
	bzero(buff2, sizeof(buff2));
	fun->name = (char *) malloc((name_len + 1) * sizeof(char));
	sprintf(fun->name, "%s%c", f_name, '\0');
	fun->arg_type = (char **) malloc(fun->arg_len * sizeof(char *));
	fun->args = (char **) malloc(fun->arg_len * sizeof(char *));
	if (fun->name == NULL || fun->arg_type == NULL || fun->args == NULL) {
		fprintf(stderr, "ERROR: Not enough memory\n");
		return NULL;
	}
	i = l = 0;
	while (*(buff1 + i)) {
		if (*(buff1 + i) == ',' || *(buff1 + i + 1) == '\0') {
			*(buff2 + l) = '\0';
			split_type_from_name(buff2, fun->arg_type + f, fun->args + f);
			f++;
			l = 0;
		} else if (*(buff1 + i) == ')') {
			break;
		} else {
			*(buff2 + l) = *(buff1 + i);
			l++;
		}
		i++;
	}
return fun;
}

int create_dummy_main(char **where, struct _test **test, size_t test_count/*char *f_name, char **args, size_t args_sz, int flags*/)
{
	struct _test *func = *test;
	int i = 0, f = 42; // f = buf write-to offset, I think it's not used anymore TODO
	char *buf;
	char *buff[50];
	int cc = 1, *ac, tc = 0;
	int alloc_size = (512 + (strlen(func->func->name) + 64) * test_count + func->func->arg_len * 64 * test_count + func->test_value_sz * 64 * test_count);
	buf = (char *) malloc(alloc_size * sizeof(char));
	if (buf == NULL) 
		return 1;
	ac = (int *) malloc(test_count * sizeof(int *));
	bzero(ac, test_count * sizeof(int *));
	sprintf(buf ,"int main(int argc, const char *argv[]) {\n\tprintf(\"\x1B[31m\");\n\t");
	if (func->flags == FLG_TEST_RET) {
	} else {
		while (tc < test_count) {
			func = *(test + tc);
			while (cc <= func->func->arg_len) {
				if (func->flags & FLG_TEST_ARG(cc)) {
					sprintf(buf + strlen(buf), "%s test_arg_%d%d;\n", *(func->func->arg_type + ac[tc]), tc, ac[tc]);
					++ac[tc];
				}
				cc++;
			}
			tc++;
		}
	}
	tc = cc = 0;
	while (tc < test_count) {
		func = *(test + tc);
		sprintf(buf + strlen(buf), "\t%s func_ret_%d = %s (", func->func->ret_t, tc, func->func->name);
		i = 0;
		while (i < func->func->arg_len) {
			if (func->flags & FLG_TEST_ARG(i + 1)) {
				sprintf(buf + strlen(buf), "&test_arg_%d%d, ", tc, cc);
				cc++;
			} else {
				sprintf(buf + strlen(buf), "%s, ", *(func->func->args + i));
			}
			i++;
		}
		tc++;
		sprintf(buf + strlen(buf) - 2, ");\n%c", 0);
	}// I THINK
	tc = 0;
	while (tc < test_count) {
		func = *(test + tc);
		if (func->flags & FLG_TEST_RET) {
			sprintf(buf + strlen(buf), "\tif (func_ret_%d == %s) {\n\t\tprintf(\"Return value of function `%s` is correct.\\n\");\n\t} else {\n\t\tprintf(\"Return value is not correct.\\n\");\n\t}\n%c", tc, *func->test_value, func->func->name ,0);
		}
		if (ac[tc]) {
			i = 0;
			while (i < func->test_value_sz - 1) {
				sprintf(buf + strlen(buf), "\tif (test_arg_%d%d == %s) {\n\t\tprintf(\"Argument #%%d of function `%s` is correct.\\n\", %d);\n\t} else {\n\t\tprintf(\"Argument #%%d of function `%s` is not correct.\\n\", %d);\n\t}\n%c", tc, i, *(func->test_value + i + 1), func->func->name, i, func->func->name, i, 0);
				i++;
			}
		}
		tc++;
	}
	sprintf(buf + strlen(buf), "\tprintf(\"\x1B[0m\");\nreturn 0;\n}");
	*where = buf;
return 0;
}

char *make_rnd_template(void)
{
	char *p = (char *) malloc(16 * sizeof(char));
	if (p == NULL) 
		return NULL;
	sprintf(p, "cxx.XXXXXX");
return p;
}

int old_create_file_for_compiling(int fp, char *main_func)
{
	int i = 0, dfp, main_sz = strlen(main_func);
	char *p = make_rnd_template();
	char buf;
	if (p == NULL) 
		return -1;
	dfp = mkstemp(p);
	free(p);
	while (read(fp, &buf, 1) > 0) {
		if (write(dfp, &buf, 1) < 0) 
			return -1;
	}
	if (write(dfp, main_func, main_sz) < 0) 
		return -1;
return dfp;
}

int copy_file(int dest, int src)
{
	char buf;
	while (read(src, &buf, 1) > 0) {
		if (write(dest, &buf, 1) == -1){
			perror("write");
			exit(EXIT_FAILURE);
			return 1;
		}
	}
return 0;
}

int create_file_for_compiling(int fp, char *main_func, char *file_name)
{
	int dfp = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0777), main_sz = strlen(main_func);
	char buf;
	if (dfp < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	copy_file(dfp, fp);
	if (write(dfp, main_func, main_sz) < 0) {
		fprintf(stderr, "Failed to write main function to file\n");
		return -1;
	}
	close(dfp);
return 0;
}

int parse_includes(char **include_list)
{
	int f = 0;
return 0;
}

char *include_missing_include(char *function_name)
{ /* todo, maybe */
	char lmao = 'u';
return NULL;
}

struct _test *parse_command_string_args(char *string)
{
	int i = 0, d = 0, s = 0, argc = 0, last_size = 1;
	char buf[256], valbuf[64] = {};
	struct _func_nfo *fun;
	struct _test *ret;
	fun = (struct _func_nfo *) malloc(1 * sizeof(struct _func_nfo));
	ret = (struct _test *) malloc(1 * sizeof(struct _test));
	ret->test_value = NULL;
	ret->test_value_sz = 0;
	ret->flags = 0;
	if (*string == SP_TEST_CHAR) {
		ret->flags |= FLG_TEST_RET;
		d = 1;
		string++;
		while (*(string) != '(') {
			(d-1)[valbuf] = *string;
			string++;
			d++;
		}
		ret->test_value_sz = 1;
		ret->test_value = (char **) malloc(ret->test_value_sz * sizeof(char **));
		*ret->test_value = (char *) malloc((d-1) * sizeof(char));
		if (ret->test_value == NULL || *ret->test_value == NULL ) {
			fprintf(stderr, "ERROR: Cannot allocate memory\n");
			exit(1);
		}
		sprintf(*ret->test_value, "%s%c", valbuf, 0);
	} 
	d = 0;
	while (*(string + d)) {
		if (*(string + d) == ',') {
			argc++;
		} else if (*(string + d) == ')') {
			break;
		} else if (*(string + d) == SP_TEST_CHAR) {
			d++;
			ret->flags |= FLG_TEST_ARG(argc + 1);
			ret->test_value_sz++;
			ret->test_value = (char **) realloc(ret->test_value, ret->test_value_sz * sizeof(char *));
			if (ret->test_value == NULL || *ret->test_value == NULL) {
				fprintf(stderr, "ERROR: Cannot allocate memory\n");
				exit(1);
			}
			s = 0;
			while(*(string + d) != ' ' && *(string + d) != ',') {
				s[valbuf] = *(string + d);
				s++;
				d++;
			}
			*(ret->test_value + ret->test_value_sz - 1) = (char *) malloc((strlen(valbuf) + 1) * sizeof(char));
			sprintf(*(ret->test_value + ret->test_value_sz - 1), "%s%c", valbuf, 0);
			continue;
		}
		d++;
	}
	s = 0;
	argc++;
	fun->arg_len = argc;
	fun->args = (char **) malloc(fun->arg_len * sizeof(char *));
	*fun->args = (char *) malloc(last_size * sizeof(char));
	if (fun->args == NULL || *fun->args == NULL) {
		fprintf(stderr, "ERROR: Not eough memory\n");
		return NULL;
	}
	d = 0;
	while (*(string + d)) {
		if (*(string + d) == ',' || *(string + d + 1) == '\0') {
			*(*(fun->args + i) + s) = '\0';
			last_size = 1;
			i++;
			fun->args = (char **) realloc(fun->args, fun->arg_len * sizeof(char *));
			*(fun->args + i) = (char *) malloc(last_size * sizeof(char));
			if (fun->args == NULL || *(fun->args + i) == NULL) {
				fprintf(stderr, "ERROR: Not enough memory\n");
				return NULL;
			}
			s = 0;
		} else if (*(string + d) != '(' && *(string + d) != ')') {
			if (s > last_size) 
				alloc_double(*(fun->args + i), &last_size);
			*(*(fun->args + i) + s) = *(string + d);
			s++;
		}
		d++;
	}
	ret->func = fun;
return ret;
}

int deal_with_return_type(char *star, char **f)
{
	int i = 0, l = 0;
	char buf[15] = {};
	while (*(star + i) != ' ') {
		*(buf + i) = *(star + i);
		i++;
	}
	while (*(star + i) == ' ') 
		i++;
	*(buf + i) = '\0';
	*f = (char *) malloc((i + 1) * sizeof(char));
	sprintf(*f, "%s%c", buf, '\0');
return i;
}

size_t *special_offsets;

int search_for_special(int fd)
{
	unsigned int i = 0, size = strlen(SP_UNIQ_CHAR);
	size_t s = 0, ss = 0;
	size_t offset_sz = 0;
	char **ret = NULL;
	char **sub;
	char buff[43] = {};
	special_offsets = NULL;
	while (read(fd, buff + s, 1) > 0) {
		if (s >= size){
			if ((sub = substr(buff, SP_UNIQ_CHAR))) {
				fprintf(stderr, "Found a special character at offset %d\n", ss + 1);
				offset_sz++;
				special_offsets = (size_t *) realloc(special_offsets, offset_sz * sizeof(size_t));
				*(special_offsets + offset_sz - 1) = ss + 1;
				lseek(fd, size, SEEK_CUR);
				bzero(buff, 43);
				ss += size;
				s = -1;
			}
		}
		if (!(s % 40) && s > 0 || *(buff + s) == '\n') {
			bzero(buff, 43);
			s = -1;
		}
		s++;
		ss++;
	}
return offset_sz;
}

struct _test **parse_command_string(int fd, size_t *ret_count) // _IM_
{
	int i = 0, s = 0, d = 0, c = 0, inc_size = 0, ret_c = 0, ret_tot;
	char buf[256], buff[128], name[256], arg[256], *i_hate_this;
	struct _incl **inc;
	struct _func_nfo *fun;
	struct _func_nfo *fun1;
	struct _test *ret;
	struct _test **rl_ret;
	if (!(ret_tot = search_for_special(fd))) {
		fprintf(stderr, "Error: Cannot find special character in the given file\n");
		return NULL;
	}
	*ret_count = ret_tot;
	rl_ret = (struct _test **) malloc(ret_tot * sizeof(struct _test *));
	while (ret_c < ret_tot) {
		*(rl_ret + ret_c) = (struct _test *) malloc(1 * sizeof(struct _test *));
		ret_c++;
	}
	ret_c = 0;
	while (ret_c < ret_tot) {
		lseek(fd, *(special_offsets + ret_c), SEEK_SET);
		ret = *(rl_ret + ret_c);
		while (read(fd, buf + i, 1) > 0) {
			if (*(buf + i) == '\n') {
				break;
			} else {
				i++;
			}
		}
		*(buf + i) = '\0';
		ret = parse_command_string_args(buf);
		while (read(fd, buff + s, 1) > 0) {
			if (*(buff + s) == '\n') {
			} else if (*(buff + s) == ')') {
				s++;
				read(fd, buff + s, 1);
				break;
			}
			s++;
		}
		*(buff + s) = '\0';
		c += deal_with_return_type(buff, &i_hate_this);
		fun = (struct _func_nfo *) malloc(1 * sizeof(struct _test));
		fun->ret_t = i_hate_this;
		while (*(buff + c) == ' ' || *(buff + c) == '\n' || *(buff + c) == '\t') 
			c++;
		while (*(buff + c) != '(' && *(buff + c) != ' ') {
			*(name + d) = *(buff + c);
			c++;
			d++;
		}
		*(name + d) = '\0';
		lseek(fd, 0, SEEK_SET);
		fun1 = parse_function(fd, name, inc, &inc_size);
		if (ret->func->arg_len != fun1->arg_len) {
			fprintf(stderr, "ERROR: function argument strings don't match\n");
			destroy_test(ret);
			destroy_func_nfo(fun1);
			return NULL;
		}
		d = ret->func->arg_len;
		ret->func->name = (char *) malloc(strlen(fun1->name) * sizeof(char));
		sprintf(ret->func->name, "%s", fun1->name);
		ret->func->arg_type = fun1->arg_type;
		ret->func->ret_t = fun->ret_t;
		while (fun1->arg_len-1) {
			free(*fun1->args++);
			fun1->arg_len--;
		}
		free(fun1->name);
		free(fun1);
		*(rl_ret + ret_c) = ret;
		ret_c++;
		bzero(buff, 128);
		bzero(buf, 256);
		s = c = d = i = 0;
	}
return rl_ret;
}

int main(int argc, const char *argv[]) {
	if (*(argv + 1) == NULL) {
		fprintf(stderr, "Usage: `%s <file name>`\n", *argv);
		exit(1);
	}
	int fd = open(*(argv + 1), O_RDONLY);
	size_t fun_c;
	struct _test **f = parse_command_string(fd, &fun_c);
	char *mainz;
	create_dummy_main(&mainz, f, fun_c);
	lseek(fd, 0, SEEK_SET);
	create_file_for_compiling(fd, mainz, SP_NEWFILE_NAME_AND_LOC);
	close(fd);
	while (fun_c) {
		destroy_test(*(f + --fun_c));
	}
	//free(mainz);
	exec_gxx(SP_NEWFILE_NAME_AND_LOC, "");
	exec("/home/kawaguchi/.UTest/fuck");
	exec("rm /home/kawaguchi/.UTest/fuck");
return 0;
}

