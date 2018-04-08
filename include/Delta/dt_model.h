// dt_model.h
#pragma once

#include <stdarg.h>
#include "dt_common.h"

typedef struct dt_Context dt_Context;
typedef uint32_t dt_Lifetime;

/* Returns the size of the dt_Context object. */
DT_FUNC size_t
dt_Context_size(
);

/* Initializes the given dt_Context object. */
DT_FUNC void
dt_Context_new(
    dt_Context* ctx
);

/* Destroys the given context object. */
DT_FUNC void
dt_Context_free(
    dt_Context* ctx
);

typedef void(*dt_model_fn)(
    dt_Context* ctx,
    va_list args
);

DT_FUNC void
dt_defer(
    dt_Context* ctx,
    dt_model_fn fn,
    ...
);

DT_FUNC void
dt_vdefer(
    dt_Context* ctx,
    dt_model_fn fn,
    va_list args
);

DT_FORCEINLINE void
dt_undefer(
    dt_Context* ctx,
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
    dt_Context* ctx
);

/* Pushes a property onto the property stack. */
DT_FUNC void
dt_push_prop(
    dt_Context* ctx,
    char const* name
);

DT_FUNC void
dt_push_elem(
    dt_Context* ctx
);

DT_FUNC void
dt_f32(
    dt_Context* ctx,
    float value
);

DT_FUNC dt_bool
dt_f32_io(
    dt_Context* ctx,
    float* io_value
);

DT_FUNC void
dt_i32(
    dt_Context* ctx,
    int32_t value
);

DT_FUNC dt_bool
dt_i32_io(
    dt_Context* ctx,
    int32_t* io_value
);

DT_FUNC void
dt_size(
    dt_Context* ctx,
    size_t value
);

DT_FUNC dt_bool
dt_size_io(
    dt_Context* ctx,
    size_t* io_value
);

