#include <stdio.h>
#include <stdlib.h>
#include "core/osl.h"

typedef struct {

    int square;

} ctx;

extern void osl_run(osl_benchmark *my_benchmark);
void naive_sqrt(void *my_ctx);
void do_nothing(void *vctx);
void *create_ctx();
void destroy_ctx(void *my_ctx);
uint64_t progress(void *my_ctx);

int main() {

    osl_impl impls[] = {
        OSL_REGISTER("naive_sqrt", naive_sqrt),
        OSL_REGISTER("do_nothing", do_nothing)
    };

    osl_benchmark *my_benchmark = malloc(sizeof(osl_benchmark));

    my_benchmark->ctx = create_ctx();
    my_benchmark->create_ctx = create_ctx;
    my_benchmark->destroy_ctx = destroy_ctx;
    my_benchmark->progress = progress;
    my_benchmark->impls = impls;
    my_benchmark->impl_count = 2;
    my_benchmark->target_sample_count = 150;
    my_benchmark->seconds = 1;

    osl_run(my_benchmark);

    free(my_benchmark);
    my_benchmark = NULL;

    return 0;
}

void naive_sqrt(void *vctx) {
    ctx *my_ctx = vctx;
    int square = my_ctx->square;
    for (int i = 0; i <= square; i++) {
        if (i * i >= square) {
            (my_ctx->square)++;
            return;
        }
    }
}

void do_nothing(void *vctx) {
    ctx *my_ctx = vctx;
    (my_ctx->square)++;
    return;
}

void *create_ctx() {
    ctx *my_ctx = (ctx *)malloc(sizeof(ctx));
    my_ctx->square = 0;
    return my_ctx;
}

void destroy_ctx(void *my_ctx) {
    free(my_ctx);
    my_ctx = NULL;
}

uint64_t progress(void *my_ctx) {
    return ((ctx *)my_ctx)->square;
}
