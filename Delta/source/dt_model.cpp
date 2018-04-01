// dt_model.cpp

#include <cassert>
#include <map>
#include <string>
#include <stack>
#include "../../include/Delta/dt_model.h"

enum ModelState {
    DT_MS_BASE,
    DT_MS_ITER,
    DT_MS_PROP_F32,
    DT_MS_ATTR_F32_MAX,
    DT_MS_ATTR_F32_MIN,
};

struct Prop_f32 {
    /* Getter */
    dt_f32_GetFn get_fn;
    void* get_fn_userdata;

    /* Setter */
    dt_f32_SetFn set_fn;
    void* set_fn_userdata;

    /* Max attribute */
    float max_const;
    dt_f32_GetFn max_get_fn;
    void* max_get_fn_userdata;

    /* Min attribute */
    float min_const;
    dt_f32_GetFn min_get_fn;
    void* min_get_fn_userdata;
};

/* Base data. */
struct State_Base {
    std::string current_name;
    std::map<std::string, Prop_f32> f32_props;
};

void State_Base_push_prop(
    dt_Context* ctx,
    char const* name
) {

}

/* Data when in the state of building an f32 property. */
struct State_PropF32 {
    Prop_f32 prop;
};

void State_Prop_push_prop(
    dt_Context* /*ctx*/,
    char const* /*name*/
) {
    assert(DT_FALSE /*Cannot push a property fore F32 properties*/);
}

/* Data when in the state of building an iterable object. */
struct State_Iter {
    dt_obj_iter_BeginFn begin_fn;
    void* begin_userdata;

    dt_obj_iter_NextFn next_fn;
    void* next_fn_userdata;

    dt_obj_iter_EndFn end_fn;
    void* end_fn_userdata;
};

struct State {
    void* data;
    ModelState state;
};

struct dt_Context {
    std::stack<State> states;
};

dt_Context* dt_Context_new(
) {
    return new dt_Context();
}

void dt_Context_free(
    dt_Context* ctx
) {
    delete ctx;
}

void dt_push_prop(
    dt_Context* ctx,
    char const* name
) {
    if (ctx->states.empty()) {
        State new_state;
        new_state.state = ModelState::DT_MS_BASE;
        new_state.data = new State_Base;
        ctx->states.push(new_state);
    }

}
