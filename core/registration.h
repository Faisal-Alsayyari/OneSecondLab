#ifndef OSL_REGISTRATION_H
#define OSL_REGISTRATION_H

#include <stdint.h>
#include <stddef.h>

typedef void (*osl_func)(void *);

typedef uint64_t (*osl_progress)(void *);

typedef struct {
	const char *name;
	osl_func func;
} osl_impl;

typedef struct {
	void *ctx;
	osl_progress progress;
	osl_impl *impls;
	size_t impl_count;
} osl_benchmark;

#define OSL_REGISTER(name, fn) {#name, fn }

#endif

