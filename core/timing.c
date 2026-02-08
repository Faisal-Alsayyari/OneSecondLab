#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "registration.h"

#define MAX_TIME (600)
#define MAX_STRING_SIZE (256)

void osl_run(osl_benchmark *benchmark);
void osl_run_individual(osl_benchmark *benchmark, osl_impl *implementation, double seconds);

void osl_run(osl_benchmark *benchmark, double seconds) {
	for (int i = 0; i < benchmark->impl_count; i++) {
		osl_run_individual(benchmark, benchmark->impls[i], seconds);
	
	}
}

void osl_run_individual(osl_benchmark *benchmark, osl_impl *implementation, double seconds) {

	if (seconds >= MAX_TIME) {
		printf("That's a LONG time... you might wanna reconsider :P\n");
	}

	// Construct the filename first

	// TODO this might not be doing what I think it's doing...
	// It might be appending the prefix to 256 garbage chars :)	

	char *filename = malloc(sizeof(char) * MAX_STRING_SIZE);
	strncpy(filename, benchmark->output_prefix, MAX_STRING_SIZE);
	strncat(filename, implementation->name, MAX_STRING_SIZE);
	strncat(filename, ".dat", MAX_STRING_SIZE);

	FILE *file = fopen(filename, "w");

	if (file == NULL) {
		printf("Error opening file on implementation %s\n", implementation->name);
		return 1;
	}


	struct timespec start, now;
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	char *output_buffer = malloc(sizeof(char) * );

	// TODO iterations will be redundant once we get proper sampling logic based on benchmark->target_sample_rate done...
	int iterations = 0;
	while (1) {
	
		implementation->func();
		iterations++;
		
		if ((iterations % benchmark->sample_rate) == 0) {
			clock_gettime(CLOCK_MONOTONIC, &now);
			double elapsed = (now.tv_sec - start.tv_sec) +
				 (now.tv_nsec - start.tv_nsec) / 1e9;

			if (elapsed >= seconds) break;

			// file writing logic
		
			uint64_t progress = benchmark->progress(benchmark->ctx);
	        fprintf(file, "%llu :: %.9f\n", progress, elapsed);	

		}
	}

	fclose(file);
	free(filename);

}
