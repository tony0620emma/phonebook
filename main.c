#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
	struct timespec diff;
	if (t2.tv_nsec-t1.tv_nsec < 0) {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
	} else {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}
	return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	int i = 0;
	char line[MAX_LAST_NAME_SIZE];
	struct timespec start, end;
	double cpu_time1, cpu_time2;

	/* check file opening */
	fp = fopen(DICT_FILE, "r");
	if (fp == NULL) {
		printf("cannot open the file\n");
		return -1;
	}

	/* build the entry */
	entry *pHead, *e;
	pHead = (entry *) malloc(sizeof(entry));
	printf("size of entry : %lu bytes\n", sizeof(entry));
	e = pHead;
	e->pNext = NULL;

#if defined(__GNUC__)
	__builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif

	clock_gettime(CLOCK_REALTIME, &start);

#if defined(OPT)
	HashTable *ht = createHashTable(TABLE_SIZE);
#endif
	while (fgets(line, sizeof(line), fp)) {
		while (line[i] != '\0')
			i++;
		line[i - 1] = '\0';
		i = 0;
#if defined(OPT)
	e = append(ht, line);
#else
        e = append(line, e);
#endif
	}
	clock_gettime(CLOCK_REALTIME, &end);
	cpu_time1 = diff_in_second(start, end);

	/* close file as soon as possible */
	fclose(fp);

	e = pHead;

	/* the givn last name to find */
	char input[MAX_LAST_NAME_SIZE] = "zyxel";
	e = pHead;
#if defined(OPT)
	char *result = malloc(sizeof(char) * MAX_LAST_NAME_SIZE);
	result = valuetoName(findName(ht, input)->lastNameValue, result);
	printf(" : %s\n", result);
	assert(0 == strcmp(result, "zyxel"));
#else
	assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
	__builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
	/* compute the execution time */
	clock_gettime(CLOCK_REALTIME, &start);
#if defined(OPT)
	findName(ht, input);
#else
	findName(input, e);
#endif
   	clock_gettime(CLOCK_REALTIME, &end);
	cpu_time2 = diff_in_second(start, end);
	FILE *output;

#if defined(OPT)
	output = fopen("opt.txt", "a");
#else 
	output = fopen("orig.txt", "a");
#endif

	fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
	fclose(output);

	printf("execution time of append() : %lf sec\n", cpu_time1);
	printf("execution time of findName() : %lf sec\n", cpu_time2);


	if (pHead->pNext) free(pHead->pNext);
	free(pHead);

#if defined(OPT)
	freeHashTable(ht);
#endif

	return 0;
}
