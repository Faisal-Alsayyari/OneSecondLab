#ifndef OSL_REGISTRATION_H
#define OSL_REGISTRATION_H

#include <stdint.h>
#include <stddef.h>

typedef void (*osl_func)(void *);

typedef uint64_t (*osl_progress)(void *);

typedef void *(*osl_create_ctx)(void);

typedef void (*osl_destroy_ctx)(void *);

typedef struct {
	const char *name;
	osl_func func;
} osl_impl;

// TODO optimize this for struct padding :)
// it shouldn't be awkward for the user since they (hopefully)
// will write the create_ctx() function so member creation order
// won't matter

typedef struct {
	void *ctx;
    osl_create_ctx create_ctx; 
    osl_destroy_ctx destroy_ctx;
    osl_progress progress;
	osl_impl *impls;
	size_t impl_count;
	size_t target_sample_count;
    size_t seconds;
} osl_benchmark;

#define OSL_REGISTER(name, fn) {#name, fn }

#endif

