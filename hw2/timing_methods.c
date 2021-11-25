// Include

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

#define IS_DIGIT(c)		(c >= '0' && c <= '9')
#define TO_DIGIT(c)		(c - '0')

static long long cpu_spd;

static inline unsigned long long getcycles(void)
{
	unsigned long low, high;
	asm volatile ("rdtsc" : "=a" (low), "=d" (high));
	return ((low) | (high) << 32);
}

static long long extract_freq(char *s, size_t len)
{
	int i = 7;
	while (!IS_DIGIT(s[i])) {i++;};

	long long res = 0;
	while (s[i] && i < len) {
		if (!IS_DIGIT(s[i])) break;
		res += TO_DIGIT(s[i]);
		i++;
		res *= 10;
	}
	res /= 10;
	return res;
}

/* Return CPU speed in MHz */
static long long cpu_speed(void)
{
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s", line);
		for (int i = 0; i < len && len > 7; i++) {
			if (line[0] == 'c' &&
				line[1] == 'p' &&
				line[2] == 'u' &&
				line[3] == ' ' &&
				line[4] == 'M' &&
				line[5] == 'H' &&
				line[6] == 'z')
			{
				return extract_freq(line, len) * pow(10, 6);		
			}
		}
    }
	return 0;

    fclose(fp);
}

/* return cycles in nanoseconds */
long long gethosttime(long long cycles)
{
	// printf("cpu spd: %llu, cycles: %llu\n", cpu_spd, cycles);
	return ((cycles*pow(10, 9)) / cpu_spd);
}

int main(int argc, char *argv[])
{
	long long res = 0;
	long long time_getcycles = 0;
	cpu_spd = cpu_speed();
	printf("Starting measuring getcycles()...\n");

	time_getcycles = getcycles();
	time_getcycles = getcycles() - time_getcycles;
	printf("cycles passed: %llu\n",time_getcycles);
	time_getcycles = gethosttime(time_getcycles);

	printf("Time of getcycles(): %llu\n", time_getcycles);

	printf("Starting measuring gettimeofday()...\n");

	long long timeofday;
	long long cycles;
	cycles = getcycles();
	timeofday = gettimeofday(NULL, NULL);
	cycles = getcycles() - cycles - time_getcycles;
	timeofday = gethosttime(cycles);

	printf("Time of getcycles(): %llu\n", timeofday);

	printf("Starting measuring inner loop with gettimeofday()...\n");

	double mean = 0;
	double st_d = 0;
	long long times[1000] = {0};
	long long ret = 0;
	struct timeval tv1;
	struct timeval tv2;
	double total = 0;
	int i = 0, j = 0, k = 0;

	for (i=0; i < 1000; i++) {
		ret = gettimeofday(&tv1, NULL);
        for (j=0; j < 100; j++) {  /* inner loop starts here */
            k = i + j;  
        }                          /* inner loop ends here */
		ret = gettimeofday(&tv2, NULL);
		times[i] = (tv2.tv_usec - tv1.tv_usec) * pow(10,3);
		total += times[i];
    }
	printf("k: %d\n", k);
	mean = total / 1000;
	total = 0;
	for (i=0; i < 1000; i++) {
		total += pow(mean - times[i], 2);
	}
	st_d = sqrt(total / 1000);

	printf("Time: mean: %f, standard dev: %f\n", mean, st_d);
	printf("Starting measuring inner loop with gethosttime()...\n");
	total = 0;
	st_d = 0;
	mean = 0;
	for (i=0; i < 1000; i++) {
		res = getcycles();
        for (j=0; j < 100; j++) {  /* inner loop starts here */
            k = i + j;  
        }                          /* inner loop ends here */
		res = getcycles() - time_getcycles - res;
		res = gethosttime(res);
		times[i] = res;
		total += times[i];
    }
	printf("k: %d\n", k);
	mean = total / 1000;
	total = 0;
	for (i=0; i < 1000; i++) {
		total += pow(mean - times[i], 2);
	}
	st_d = sqrt(total / 1000);

	printf("Time: mean: %f, standard dev: %f\n", mean, st_d);

	return 0;
}


