// dt_template.h
#pragma once

#include "dt_layout.h"

typedef char const* dt_ViewModel_Expr_t;
typedef struct dt_ViewModel dt_ViewModel;
typedef struct dt_template_Property dt_template_Property;
typedef struct dt_template_EdgeBox dt_template_EdgeBox;
typedef struct dt_template_CornerBox dt_template_CornerBox;
typedef enum dt_template_ElementType dt_template_ElementType;
typedef struct dt_template_Element dt_template_Element;

typedef struct dt_template_BlockBorder dt_template_BlockBorder;
typedef struct dt_template_Box dt_template_Box;
typedef struct dt_template_BoxChild dt_template_BoxChild;
typedef struct dt_template_Stack dt_template_Stack;
typedef struct dt_template_StackChild dt_template_StackChild;
typedef struct dt_template_Window dt_template_Window;

/* ViewModel function used to evaluate boolean properties and predicates. */
typedef dt_bool_t(*dt_ViewModel_Eval_bool_fn)(
	void* user_data,
	dt_ViewModel_Expr_t expr
);

/* ViewModel function used to evaluate float properties. */
typedef float(*dt_ViewModel_Eval_f32_fn)(
	void* user_data,
	dt_ViewModel_Expr_t expr
);

/* ViewModel function used to evaluate int32 properties. */
typedef int32_t(*dt_ViewModel_Eval_i32_fn)(
	void* user_data,
	dt_ViewModel_Expr_t expr
);

typedef dt_Color_t(*dt_ViewModel_Eval_Color_fn)(
	void* user_data,
	dt_ViewModel_Expr_t expr
);

struct dt_ViewModel {
	void* user_data;
	dt_ViewModel_Eval_bool_fn eval_bool;
	dt_ViewModel_Eval_f32_fn eval_f32;
	dt_ViewModel_Eval_i32_fn eval_i32;
	dt_ViewModel_Eval_Color_fn eval_color;
};

struct dt_template_Property {
	union {
		dt_bool_t fixed_bool;
		float fixed_f32;
		int32_t fixed_i32;
		dt_Color_t fixed_color;
		dt_ViewModel_Expr_t expr;
	};
	dt_bool_t is_expr;
};

DT_FUNC dt_template_Property dt_template_Property_expr(
	dt_ViewModel_Expr_t expr
);

DT_FUNC dt_template_Property dt_template_Property_bool(
	dt_bool_t value
);

DT_FUNC dt_bool_t dt_template_Property_get_bool(
	dt_template_Property property,
	dt_ViewModel const* view_model
);

DT_FUNC dt_template_Property dt_template_Property_f32(
	float value
);

DT_FUNC float dt_template_Property_get_f32(
	dt_template_Property property,
	dt_ViewModel const* view_model
);

DT_FUNC dt_template_Property dt_template_Property_i32(
	int32_t value
);

DT_FUNC int32_t dt_template_Property_get_i32(
	dt_template_Property property,
	dt_ViewModel const* view_model
);

DT_FUNC dt_template_Property dt_template_Property_color(
	dt_Color_t value
);

DT_FUNC dt_Color_t dt_template_Property_get_color(
	dt_template_Property property,
	dt_ViewModel const* view_model
);

struct dt_template_EdgeBox {
	dt_template_Property top;
	dt_template_Property right;
	dt_template_Property bottom;
	dt_template_Property left;
};

DT_FUNC void dt_template_EdgeBox_init_default(
	dt_template_EdgeBox* edge_box
);

DT_FUNC dt_layout_EdgeBox dt_template_EdgeBox_get_layout(
	dt_template_EdgeBox const* edge_box,
	dt_ViewModel const* view_model
);

struct dt_template_CornerBox {
	dt_template_Property top_left;
	dt_template_Property top_right;
	dt_template_Property bottom_left;
	dt_template_Property bottom_right;
};

DT_FUNC void dt_template_CornerBox_init(
	dt_template_CornerBox* corner_box
);

DT_FUNC dt_layout_CornerBox dt_template_CornerBox_get_layout(
	dt_template_CornerBox const* corner_box,
	dt_ViewModel const* view_model
);

enum dt_template_ElementType {
	DT_TEMPLATE_EMPTY,
	DT_TEMPLATE_BLOCK_BORDER,
	DT_TEMPLATE_BOX,
	DT_TEMPLATE_STACK,
};

struct dt_template_Element {
	void const* data;
	dt_template_ElementType type;
};

DT_FUNC dt_layout_Element dt_template_Element_generate_layout(
	dt_template_Element template_element,
	dt_ViewModel const* view_model,
	dt_Allocator* layout_allocator,
	dt_layout_Bounds* out_bounds
);

/*
 *
 * BLOCK BORDER
 *
 */

struct dt_template_BlockBorder {
	dt_ViewModel_Expr_t element_predicate_expr;
	dt_template_Element template_element;
	dt_template_Property min_width;
	dt_template_Property max_width;
	dt_template_Property min_height;
	dt_template_Property max_height;
	dt_template_EdgeBox padding;
	dt_template_EdgeBox border_thickness;
	dt_template_CornerBox corner_radii;
	dt_template_Property background_color;
	dt_template_Property border_color;
};

DT_FUNC void dt_template_BlockBorder_init(
	dt_template_BlockBorder* out_block_border,
	dt_template_Element* out_element
);

DT_FUNC dt_layout_BlockBorder* dt_template_BlockBorder_generate_layout(
	dt_template_BlockBorder const* block_border,
	dt_ViewModel const* view_model,
	dt_Allocator* layout_allocator,
	dt_layout_Bounds* out_bounds
);

/*
 *
 * BOX
 *
 */

struct dt_template_Box {
	dt_template_BoxChild const* first_child;
	dt_template_Property min_width;
	dt_template_Property max_width;
	dt_template_Property min_height;
	dt_template_Property max_height;
};

struct dt_template_BoxChild {
	dt_template_BoxChild const* next_child;
	dt_ViewModel_Expr_t predicate_expr;
	dt_template_Element template_element;
	dt_template_EdgeBox margin;
	dt_template_Property horizontal_alignment;
	dt_template_Property vertical_alignment;
};

DT_FUNC void dt_template_Box_init(
	dt_template_Box* out_box,
	dt_template_Element* out_element
);

DT_FUNC void dt_template_BoxChild_init(
	dt_template_BoxChild* out_box_child,
	dt_template_BoxChild const** out_child_ptr
);

DT_FUNC void dt_template_Box_set_width(
	dt_template_Box* box,
	float width
);

DT_FUNC void dt_template_Box_set_height(
	dt_template_Box* box,
	float height
);

DT_FUNC dt_layout_Box* dt_template_Box_generate_layout(
	dt_template_Box const* box,
	dt_ViewModel const* view_model,
	dt_Allocator* layout_allocator,
	dt_layout_Bounds* out_bounds
);

/*
 *
 * STACK
 *
 */

struct dt_template_Stack {
	dt_template_StackChild const* first_child;
	dt_template_Property min_width;
	dt_template_Property max_width;
	dt_template_Property min_height;
	dt_template_Property max_height;
	dt_template_Property stack_direction;
	dt_template_Property last_child_fill;
};

struct dt_template_StackChild {
	dt_template_StackChild const* next_child;
	dt_ViewModel_Expr_t predicate_expr;
	dt_template_Element template_element;
	dt_template_EdgeBox margin;
	dt_template_Property cross_alignment;
};

DT_FUNC void dt_template_Stack_init(
	dt_template_Stack* out_stack,
	dt_template_Element* out_element
);

DT_FUNC void dt_template_StackChild_init(
	dt_template_StackChild* out_stack_child,
	dt_template_StackChild const** out_child_ptr
);

DT_FUNC dt_layout_Stack* dt_template_Stack_generate_layout(
	dt_template_Stack const* stack,
	dt_ViewModel const* view_model,
	dt_Allocator* layout_allocator,
	dt_layout_Bounds* out_bounds
);

/*
*
* WINDOW
*
*/

struct dt_template_Window {
	dt_template_Box content_box;
	dt_template_Property background_color;
};

typedef dt_template_BoxChild dt_template_WindowChild;

DT_FUNC void dt_template_Window_init(
	dt_template_Window* out_window
);

DT_FUNC void dt_template_WindowChild_init(
	dt_template_WindowChild* out_window_child,
	dt_template_WindowChild const** out_child_ptr
);

DT_FUNC void dt_template_Window_generate_layout(
	dt_template_Window const* window,
	dt_ViewModel const* view_model,
	dt_Allocator* layout_allocator,
	dt_layout_Window* out_layout_window
);
