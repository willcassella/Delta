// model.h
#pragma once

#include <stdarg.h>
#include "common.h"

/* Returns the size of the dt_Context object. */
DT_FUNC size_t
dt_Context_size(
);

/* Initializes the given dt_Context object. */
DT_FUNC void
dt_Context_new(
    struct dt_Context* ctx
);

/* Destroys the given context object. */
DT_FUNC void
dt_Context_free(
    struct dt_Context* ctx
);

typedef void(*dt_model_fn)(
    struct dt_Context* ctx,
    va_list args
);

DT_FUNC void
dt_defer(
    struct dt_Context* ctx,
    dt_model_fn fn,
    ...
);

DT_FUNC void
dt_vdefer(
    struct dt_Context* ctx,
    dt_model_fn fn,
    va_list args
);

DT_FORCEINLINE void
dt_undefer(
    struct dt_Context* ctx,
    dt_model_fn fn,
    ...
) {
    va_list args;
    va_start(args, fn);
    fn(ctx, args);
    va_end(args);
}

DT_FUNC void
dt_pop(
    struct dt_Context* ctx
);

/* Pushes a property onto the property stack. */
DT_FUNC void
dt_push_prop(
    struct dt_Context* ctx,
    char const* name
);

DT_FUNC void
dt_push_elem(
    struct dt_Context* ctx
);

DT_FUNC void
dt_bool(
    struct dt_Context* ctx,
    bool value
);

DT_FUNC bool
dt_bool_io(
    struct dt_Context* ctx,
    bool* io_value
);

DT_FUNC void
dt_i32(
    struct dt_Context* ctx,
    int32_t value
);

DT_FUNC bool
dt_i32_io(
    struct dt_Context* ctx,
    int32_t* io_value
);

DT_FUNC void
dt_f32(
    struct dt_Context* ctx,
    float value
);

DT_FUNC bool
dt_f32_io(
    struct dt_Context* ctx,
    float* io_value
);

DT_FUNC void
dt_size(
    struct dt_Context* ctx,
    size_t value
);

DT_FUNC bool
dt_size_io(
    struct dt_Context* ctx,
    size_t* io_value
);

DT_FUNC void
dt_str(
    struct dt_Context* ctx,
    char const* value,
    size_t len
);
