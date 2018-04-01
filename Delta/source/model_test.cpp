// Model_test.cpp

#include <math.h>
#include <vector>
#include "../../include/Delta/dt_model.h"

struct Vec3 {
    float x, y, z;
};

float Vec3_get_x(
    void* /*userdata*/,
    void* vec
) {
    return ((Vec3 const*)vec)->x;
}

void Vec3_set_x(
    void* /*userdata*/,
    void* vec,
    float v
) {
    ((Vec3*)vec)->x = v;
}

float Vec3_get_y(
    void* /*userdata*/,
    void* vec
) {
    return ((Vec3 const*)vec)->y;
}

void Vec3_set_y(
    void* /*userdata*/,
    void* vec,
    float v
) {
    ((Vec3*)vec)->y = v;
}

float Vec3_get_z(
    void* /*userdata*/,
    void* vec
) {
    return ((Vec3 const*)vec)->z;
}

void Vec3_set_z(
    void* /*userdata*/,
    void* vec,
    float v
) {
    ((Vec3*)vec)->z = v;
}

float Vec3_get_length(
    void* /*userdata*/,
    void* vec
) {
    Vec3 const* const v = (Vec3 const*)vec;
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

void Vec3_model(
    dt_Context* const ctx
) {
    //dt_push_prop(ctx, "x");
    //dt_f32_get(ctx, &Vec3_get_x, NULL);
    //dt_f32_set(ctx, &Vec3_set_x, NULL);
    //dt_pop(ctx);

    //dt_push_prop(ctx, "y");
    //dt_f32_get(ctx, &Vec3_get_y, NULL);
    //dt_f32_set(ctx, &Vec3_set_y, NULL);
    //dt_pop(ctx);

    //dt_push_prop(ctx, "z");
    //dt_f32_get(ctx, &Vec3_get_z, NULL);
    //dt_f32_set(ctx, &Vec3_set_z, NULL);
    //dt_pop(ctx);

    //dt_push_prop(ctx, "length");
    //dt_f32_get(ctx, &Vec3_get_length, NULL);
    //dt_pop(ctx);
}

struct Vec3_iter {
    std::vector<Vec3>::iterator current;
    std::vector<Vec3>::iterator end;
};

void* Vec3_vec_iter_begin(
    void* /*userdata*/,
    void* vec
) {
    auto* const v = (std::vector<Vec3>*)vec;

    auto* result = new Vec3_iter{};
    result->current = v->begin();
    result->end = v->end();
    return result;
}

void* Vec3_vec_iter_next(
    void* /*userdata*/,
    void* iter
) {
    auto* const i = (Vec3_iter*)iter;
    auto* const result = &*i->current;
    ++i->current;
    return result;
}

dt_bool Vec3_vec_iter_end(
    void* /*userdata*/,
    void* iter
) {
    auto* const i = (Vec3_iter*)iter;
    if (i->current == i->end) {
        delete i;
        return DT_TRUE;
    }

    return DT_FALSE;
}

void Vec3_vec_model(
    dt_Context* const ctx
) {
    //dt_obj_iter_begin(ctx, &Vec3_vec_iter_begin, NULL);
    //dt_obj_iter_next(ctx, &Vec3_vec_iter_next, NULL);
    //dt_obj_iter_end(ctx, &Vec3_vec_iter_end, NULL);
    //Vec3_model(ctx);
}
