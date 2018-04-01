// dt_model_v2.h
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct dt_Context dt_Context;
typedef uint32_t dt_Lifetime;

/* Returns the size of the dt_Context object. */
size_t dt_Context_size(
);

/* Initializes the given dt_Context object. */
void dt_Context_new(
    dt_Context* ctx
);

/* Destroys the given context object. */
void dt_Context_free(
    dt_Context* ctx
);

void dt_pop(
    dt_Context* ctx
);

/* Pushes a property onto the property stack. */
void dt_push_prop(
    dt_Context* ctx,
    char const* name
);

void dt_f32_const(
    dt_Context* ctx,
    float value
);

typedef void(*dt_begin_fn)(
    dt_Context* ctx,
    void* userdata
);

typedef bool(*dt_next_fn)(
    dt_Context* ctx,
    void* userdata
);

/* Creates a new lifetime that is dependent on the given lifetimes. */
dt_Lifetime dt_new_lt(
    dt_Context* ctx,
    dt_Lifetime const* parents,
    size_t num_parents
);
