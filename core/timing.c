#define _POSIX_C_SOURCE 200809L
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "osl.h"

#define MAX_TIME (30)
#define FILENAME_LENGTH (256)
#define FPS (30)
#define GIF_LENGTH (10)

void osl_run(osl_benchmark *benchmark);

void osl_run_individual(osl_benchmark *benchmark, osl_impl *implementation, double seconds);

void osl_run(osl_benchmark *benchmark) {
   
	if (benchmark->seconds >= MAX_TIME) {
		printf("ERROR: for your sanity, the current maximum benchmark time supported is 30 seconds.\n");
	    return;
    }

    printf("--- Benchmark Started ---\n");

   // This forces all .dat files to target 30 FPS.
   // In future versions we may allow the user to define FPS
   // It also assumes the final GIF length will be 10 seconds, but this can be changed in the editor

   benchmark->target_sample_count = FPS * GIF_LENGTH * benchmark->seconds; 

    for (size_t i = 0; i < benchmark->impl_count; i++) {
		void *ctx = benchmark->create_ctx();
        benchmark->ctx = ctx;
        osl_run_individual(benchmark, &benchmark->impls[i], (double)benchmark->seconds);
	    benchmark->destroy_ctx(benchmark->ctx);

	}

    printf("--- Benchmark Completed ---\n");

}

void osl_run_individual(osl_benchmark *benchmark, osl_impl *implementation, double seconds) {

    char filename[FILENAME_LENGTH];

    if (snprintf(filename, FILENAME_LENGTH, "osl_%s.dat", implementation->name) < 0) {
        printf("Filename encoding error on %s :(\n", implementation->name);
        return;
    }

	FILE *file = fopen(filename, "w");

	if (file == NULL) {
		printf("Error opening file on implementation %s\n", implementation->name);
		return;
	}


	struct timespec start;
    struct timespec before;
    struct timespec after;

    clock_gettime(CLOCK_MONOTONIC, &start);
    before = start;

    // All timing is in nanoseconds.

    double seconds_ns = seconds * 1e9;
    int datapoints_collected = 0;
    double elapsed_ns = 0;
    double time_since_last_write = 0;
    
    double target_write_interval = (double)(seconds_ns / benchmark->target_sample_count); 
    while (elapsed_ns <= seconds_ns) {

        // time only the function call

        clock_gettime(CLOCK_MONOTONIC, &before);
		implementation->func(benchmark->ctx);
        clock_gettime(CLOCK_MONOTONIC, &after);
        
        elapsed_ns = (double)(after.tv_sec - start.tv_sec) * 1e9 + (double)(after.tv_nsec - start.tv_nsec);
        double delta = (double)(after.tv_sec - before.tv_sec) * 1e9 + (double)(after.tv_nsec - before.tv_nsec);

        time_since_last_write += delta;

        if (time_since_last_write >= target_write_interval) {

			// file writing
			uint64_t progress = benchmark->progress(benchmark->ctx);
	        fprintf(file, "%lu :: %.9f\n", progress, (elapsed_ns / 1e9));	

            time_since_last_write = 0;
            datapoints_collected++;
		}
	}

    printf("Implementation %s successfully wrote %d datapoints to %s\n", implementation->name, datapoints_collected, filename);

	fclose(file);
    file = NULL;

}
