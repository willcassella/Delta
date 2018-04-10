// Vec3.h

#include <math.h>
#include "../include/Delta/model.h"

struct Vec3 {
    float x;
    float y;
    float z;
};

inline float
Vec3_length(
    struct Vec3 const vec
) {
    return sqrtf(vec.x * vec.y * vec.z);
}

void
Vec3_model_comp(
    dt_Context* const ctx,
    va_list args
) {
    float* const v = va_arg(args, float*);
    dt_f32_io(ctx, v);
}

void
Vec3_model_length(
    dt_Context* const ctx,
    va_list args
) {
    struct Vec3* const vec = va_arg(args, struct Vec3*);
    dt_f32(ctx, Vec3_length(*vec));
}
    

void
Vec3_model(
    dt_Context* const ctx,
    va_list args
) {
    struct Vec3* const vec = va_arg(args, struct Vec3*);

    dt_push_prop(ctx, "length");
    dt_defer(ctx, &Vec3_model_length, vec);
    dt_pop(ctx);

    dt_push_prop(ctx, "x");
    dt_defer(ctx, &Vec3_model_comp, &vec->x);
    dt_pop(ctx);

    dt_push_prop(ctx, "y");
    dt_defer(ctx, &Vec3_model_comp, &vec->y);
    dt_pop(ctx);

    dt_push_prop(ctx, "z");
    dt_defer(ctx, &Vec3_model_comp, &vec->z);
    dt_pop(ctx);
}

void
Vec3Array_iter(
    dt_Context* const ctx,
    va_list args
) {
    struct Vec3* const values = va_arg(args, struct Vec3*);
    size_t const len = va_arg(args, size_t);

    if (!len) {
        return;
    }

    dt_push_elem(ctx);
    dt_undefer(ctx, &Vec3_model, values);
    dt_pop(ctx);

    dt_defer(ctx, &Vec3Array_iter, values + 1, len - 1);
}

void
Vec3Array_model(
    dt_Context* const ctx,
    va_list args
) {
    struct Vec3* const first = va_arg(args, struct Vec3*);
    size_t const len = va_arg(args, size_t);

    dt_push_prop(ctx, "len");
    dt_size(ctx, len);
    dt_pop(ctx);

    dt_push_prop(ctx, "values");
    dt_defer(ctx, &Vec3Array_iter, first, len);
    dt_pop(ctx);
}

