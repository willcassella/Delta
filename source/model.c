#include <memory.h>
#include "../include/Delta/model.h"

struct dt_Context {

};

size_t
dt_Context_size(
) {
    return sizeof(struct dt_Context);
}

void
dt_Context_new(
    dt_Context *ctx
) {
    memset(ctx, 0, sizeof(*ctx));
}

void
dt_Context_free(
    dt_Context *ctx
) {
}

void
dt_defer(
    dt_Context *ctx,
    dt_model_fn fn,
    ...
) {
    va_list args;
    va_start(args, fn);
    dt_vdefer(ctx, fn, args);
    va_end(args);
}

void
dt_vdefer(
    dt_Context *ctx,
    dt_model_fn fn,
    va_list args
) {

}
