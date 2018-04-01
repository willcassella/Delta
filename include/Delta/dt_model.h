// dt_model.h

#include <stdint.h>
#include "dt_common.h"

typedef struct dt_Context dt_Context;

DT_FUNC dt_Context* dt_Context_new(
);

DT_FUNC void dt_Context_free(
    dt_Context* ctx
);

DT_FUNC void dt_push_prop(
    dt_Context* ctx,
    char const* name
);

DT_FUNC void dt_pop(
    dt_Context* ctx
);

typedef void*(*dt_obj_GetFn)(
    void* userdata,
    void* obj
);

DT_FUNC void dt_obj_get(
    dt_Context* ctx,
    dt_obj_GetFn get_fn,
    void* userdata
);

typedef void*(*dt_obj_iter_BeginFn)(
    void* userdata,
    void* obj
);

typedef void*(*dt_obj_iter_NextFn)(
    void* userdata,
    void* iter
);

typedef dt_bool(*dt_obj_iter_EndFn)(
    void* userdata,
    void* iter
);

DT_FUNC void dt_obj_iter_begin(
    dt_Context* ctx,
    dt_obj_iter_BeginFn begin_fn,
    void* userdata
);

DT_FUNC void dt_obj_iter_next(
    dt_Context* ctx,
    dt_obj_iter_NextFn next_fn,
    void* userdata
);

DT_FUNC void dt_obj_iter_end(
    dt_Context* ctx,
    dt_obj_iter_EndFn end_fn,
    void* userdata
);

DT_FUNC void dt_f32_const(
    dt_Context* ctx,
    float value
);

typedef float(*dt_f32_GetFn)(
    void* userdata,
    void* obj
);

DT_FUNC void dt_f32_get(
    dt_Context* ctx,
    dt_f32_GetFn get_fn,
    void* userdata
);

typedef void(*dt_f32_SetFn)(
    void* userdata,
    void* obj,
    float v
);

DT_FUNC void dt_f32_set(
    dt_Context* ctx,
    dt_f32_SetFn set_fn,
    void* userdata
);

DT_FUNC void dt_push_attr_f32_max(
    dt_Context* ctx
);

DT_FUNC void dt_push_attr_f32_min(
    dt_Context* ctx
);
