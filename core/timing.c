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

void osl_run(osl_benchmark *benchmark, double seconds);
void osl_run_individual(osl_benchmark *benchmark, osl_impl *implementation, double seconds);

void osl_run(osl_benchmark *benchmark, double seconds) {
   
	if (seconds >= MAX_TIME) {
		printf("ERROR: for your sanity, the current maximum benchmark time supported is 30 seconds.\n");
	}

    printf("--- Benchmark Started ---");

   // This forces all .dat files to target 30 FPS.
   // In future versions we may allow the user to define FPS

   benchmark->target_sample_count = FPS * seconds; 

    for (size_t i = 0; i < benchmark->impl_count; i++) {
		void *ctx = benchmark->create_ctx();
        benchmark->ctx = ctx;
        osl_run_individual(benchmark, &benchmark->impls[i], seconds);
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


	struct timespec start, now;
	clock_gettime(CLOCK_MONOTONIC, &start);

    // sidenote: elapsed is in units of seconds (s)
    //           time_since_last write and target_write interval are
    //           in units of nanoseconds (ns)

    int datapoints_collected = 0;
    double elapsed = 0;
    double time_since_last_write = 0;
    double target_write_interval = (seconds / benchmark->target_sample_count) * 1e9; 
    while (elapsed < seconds) {
	
		implementation->func(benchmark->ctx);

        clock_gettime(CLOCK_MONOTONIC, &now);
	    double elapsed = (now.tv_sec - start.tv_sec) +
				         (now.tv_nsec - start.tv_nsec) / 1e9;
        time_since_last_write += elapsed;
		
        if (time_since_last_write >= target_write_interval) {

			// file writing
			uint64_t progress = benchmark->progress(benchmark->ctx);
	        fprintf(file, "%lu :: %.9f\n", progress, elapsed);	

            time_since_last_write = 0;
            datapoints_collected++;
		}
	}

    printf("Implementation %s successfully wrote %d datapoints to %s\n", implementation->name, datapoints_collected, filename);

	fclose(file);
    file = NULL;

}
