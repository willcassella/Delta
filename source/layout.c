// dt_layout.c

#include <math.h>
#include <assert.h>
#include "../include/Delta/layout.h"
#include "../include/Delta/render_command.h"
#include "../private/dt_build_private.h"

static void
dt_layout_generate_rect(
	struct dt_Allocator* const allocator,
	float const x,
	float const y,
	float const width,
	float const height,
	dt_Color const color
) {
	struct dt_RenderCommand* const commands = dt_Allocator_allocate(allocator, sizeof(struct dt_RenderCommand[2]));
	commands[0].coords[0] = x;
	commands[0].coords[1] = y;
	commands[0].coords[2] = x;
	commands[0].coords[3] = y + height;
	commands[0].coords[4] = x + width;
	commands[0].coords[5] = y + height;
	commands[0].color = color;

	commands[1].coords[0] = x;
	commands[1].coords[1] = y;
	commands[1].coords[2] = x + width;
	commands[1].coords[3] = y + height;
	commands[1].coords[4] = x + width;
	commands[1].coords[5] = y;
	commands[1].color = color;
}

DT_FORCEINLINE static void
dt_layout_align_top_compact(
	float const min_space,
	float const computed_space,
	float const top_margin,
	float const bottom_margin,
	float* DT_RESTRICT const io_pos,
	float* DT_RESTRICT const io_space,
	float* DT_RESTRICT const out_remain_pos,
	float* DT_RESTRICT const out_remain_space
) {
	DT_UNUSED(min_space);
	DT_UNUSED(bottom_margin);

	float const consumed_space = top_margin + computed_space;
	*out_remain_pos = *io_pos + consumed_space;
	*out_remain_space = *io_space - consumed_space;
	*io_pos += top_margin;
	*io_space = computed_space;
}

DT_FORCEINLINE static void
dt_layout_align_bottom_compact(
	float const min_space,
	float const computed_space,
	float const top_margin,
	float const bottom_margin,
	float* DT_RESTRICT const io_pos,
	float* DT_RESTRICT const io_space,
	float* DT_RESTRICT const out_remain_space
) {
	DT_UNUSED(min_space);
	DT_UNUSED(top_margin);

	float const consumed_space = computed_space + bottom_margin;
	*out_remain_space = *io_space - consumed_space;
	*io_pos += *out_remain_space;
	*io_space = computed_space;
}

DT_FORCEINLINE static void
dt_layout_align_top_stretch(
	float const min_space,
	float const max_space,
	float const computed_space,
	float const top_margin,
	float const bottom_margin,
	float* DT_RESTRICT const io_pos,
	float* DT_RESTRICT const io_space
) {
	DT_UNUSED(min_space);
	DT_UNUSED(computed_space);

	float const available_space = *io_space - top_margin - bottom_margin;
	*io_pos += top_margin;
	*io_space = fminf(available_space, max_space);
}

DT_FORCEINLINE static void
dt_layout_align_bottom_stretch(
	float const min_space,
	float const max_space,
	float const computed_space,
	float const top_margin,
	float const bottom_margin,
	float* DT_RESTRICT const io_pos,
	float* DT_RESTRICT const io_space
) {
	DT_UNUSED(min_space);
	DT_UNUSED(computed_space);

	float const available_space = *io_space - top_margin - bottom_margin;
	float const space = fminf(available_space, max_space);
	*io_pos += *io_space - bottom_margin - space;
	*io_space = space;
}

static void
dt_layout_align_x(
	float const max_width,
	float const left_padding,
	float const right_padding,
	dt_HorizontalAlignment const horizontal_alignment,
	float* DT_RESTRICT const io_x,
	float* DT_RESTRICT const io_width
) {
	float const allocated_width = *io_width;
	float const available_width = fmaxf(allocated_width - left_padding - right_padding, 0.f);
	float const result_width = fminf(max_width, available_width);
	*io_width = result_width;

	float const x = *io_x;
	switch (horizontal_alignment)
	{
	case DT_HALIGN_CENTER:
		*io_x = x + left_padding + available_width / 2 - result_width / 2;
		break;

	case DT_HALIGN_LEFT:
		*io_x = x + left_padding;
		break;

	case DT_HALIGN_RIGHT:
		*io_x = x + allocated_width - left_padding - result_width;
		break;

	default:
		assert(false /* Unknown dt_HorizontalAlignment_t value */);
	}
}

static void
dt_layout_align_y(
	float const max_height,
	float const top_padding,
	float const bottom_padding,
	dt_VerticalAlignment const vertical_alignment,
	float* DT_RESTRICT const io_y,
	float* DT_RESTRICT const io_height
) {
	float const allocated_height = *io_height;
	float const available_height = fmaxf(allocated_height - top_padding - bottom_padding, 0.f);
	float const result_height = fminf(max_height, available_height);
	*io_height = result_height;

	float const y = *io_y;
	switch (vertical_alignment)
	{
	case DT_VALIGN_CENTER:
		*io_y = y + top_padding + available_height / 2 - result_height / 2;
		break;

	case DT_VALIGN_TOP:
		*io_y = y + top_padding;
		break;

	case DT_VALIGN_BOTTOM:
		*io_y = y + allocated_height - bottom_padding - result_height;
		break;

	default:
		assert(false /* Unknown dt_VerticalAlignment_t value */);
	}
}

void
dt_layout_Bounds_validate(
	struct dt_layout_Bounds* const io_bounds
) {
	float computed_width = io_bounds->computed_width;
	float computed_height = io_bounds->computed_height;

	computed_width = fmaxf(computed_width, io_bounds->min_width);
	computed_height = fmaxf(computed_height, io_bounds->min_height);
	computed_width = fminf(computed_width, io_bounds->max_width);
	computed_height = fminf(computed_height, io_bounds->max_height);

	io_bounds->computed_width = computed_width;
	io_bounds->computed_height = computed_height;
}

void
dt_layout_Element_arrange_content(
	struct dt_layout_Element const layout_element,
	struct dt_layout_Bounds* const io_bounds
) {
	switch (layout_element.type)
	{
	case DT_LAYOUT_EMPTY:
		io_bounds->computed_width = 0.f;
		io_bounds->computed_height = 0.f;
		break;

	case DT_LAYOUT_BLOCK_BORDER:
		dt_layout_BlockBorder_arrange_content(layout_element.data, io_bounds);
		break;

	case DT_LAYOUT_BOX:
		dt_layout_Box_arrange_content(layout_element.data, io_bounds);
		break;

	case DT_LAYOUT_STACK:
		dt_layout_Stack_arrange_content(layout_element.data, io_bounds);
		break;

	default:
		assert(false /* Unkown dt_layout_Element_t element type */);
	}
}

void
dt_layout_Element_generate_commands(
	struct dt_layout_Element const layout_element,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox const content_box
) {
	switch (layout_element.type)
	{
	case DT_LAYOUT_EMPTY:
		break;

	case DT_LAYOUT_BLOCK_BORDER:
		dt_layout_BlockBorder_generate_commands(layout_element.data, command_allocator, content_box);
		break;

	case DT_LAYOUT_BOX:
		dt_layout_Box_generate_commands(layout_element.data, command_allocator, content_box);
		break;

	case DT_LAYOUT_STACK:
		dt_layout_Stack_generate_commands(layout_element.data, command_allocator, content_box);
		break;

	default:
		assert(false /* Unknown dt_layout_Element_t element type */);
	}
}

/*
 *
 * BLOCK BORDER
 *
 */

void
dt_layout_BlockBorder_arrange_content(
	struct dt_layout_BlockBorder const* const block_border,
	struct dt_layout_Bounds* const io_bounds)
{
	float const add_width = block_border->border_thickness.left + block_border->border_thickness.right + block_border->padding.left + block_border->padding.right;
	float const add_height = block_border->border_thickness.top + block_border->border_thickness.bottom + block_border->padding.top + block_border->padding.bottom;
	io_bounds->computed_width = add_width + block_border->element_bounds.computed_width;
	io_bounds->computed_height = add_height + block_border->element_bounds.computed_height;
}

void
dt_layout_BlockBorder_generate_commands(
	struct dt_layout_BlockBorder const* const block_border,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox content_box
) {
	// Generate command for content
	dt_layout_generate_rect(command_allocator, content_box.x, content_box.y, content_box.width, content_box.height, block_border->background_color);

	// Generate commands for border
	dt_layout_generate_rect(command_allocator, content_box.x, content_box.y, content_box.width, block_border->border_thickness.top, block_border->border_color);
	dt_layout_generate_rect(command_allocator, content_box.x + content_box.width - block_border->border_thickness.right, content_box.y, block_border->border_thickness.right, content_box.height, block_border->border_color);
	dt_layout_generate_rect(command_allocator, content_box.x, content_box.y + content_box.height - block_border->border_thickness.bottom, content_box.width, block_border->border_thickness.bottom, block_border->border_color);
	dt_layout_generate_rect(command_allocator, content_box.x, content_box.y, block_border->border_thickness.left, content_box.height, block_border->border_color);

	// Generate commands for contained element
	content_box.x += block_border->border_thickness.left + block_border->padding.left;
	content_box.width -= block_border->border_thickness.left + block_border->border_thickness.right + block_border->padding.left + block_border->padding.right;
	content_box.y += block_border->border_thickness.top + block_border->padding.top;
	content_box.height -= block_border->border_thickness.top + block_border->border_thickness.bottom + block_border->padding.top + block_border->padding.bottom;
	dt_layout_Element_generate_commands(block_border->layout_element, command_allocator, content_box);
}

/*
 *
 * BOX
 *
 */

void
dt_layout_Box_arrange_content(
	struct dt_layout_Box const* const layout_box,
	struct dt_layout_Bounds* const io_bounds
) {
	float content_width = 0.f;
	float content_height = 0.f;
	for (struct dt_layout_BoxChild const* child = layout_box->first_child; child != NULL; child = child->next_child)
	{
		// Compute required width and height
		float const child_width = child->margin.left + child->bounds.computed_width + child->margin.right;
		float const child_height = child->margin.top + child->bounds.computed_height + child->margin.bottom;

		content_width = fmaxf(content_width, child_width);
		content_height = fmaxf(content_height, child_height);
	}

	io_bounds->computed_width = fmaxf(content_width, io_bounds->min_width);
	io_bounds->computed_height = fmaxf(content_height, io_bounds->min_height);
}

void
dt_layout_Box_generate_commands(
	struct dt_layout_Box const* const layout_box,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox const content_box
) {
	// Generate child commands
	for (struct dt_layout_BoxChild const* child = layout_box->first_child; child != NULL; child = child->next_child)
	{
		struct dt_layout_ContentBox child_content_box = content_box;
		dt_layout_align_x(child->bounds.max_width, child->margin.left, child->margin.right, child->horizontal_alignment, &child_content_box.x, &child_content_box.width);
		dt_layout_align_y(child->bounds.max_height, child->margin.top, child->margin.bottom, child->vertical_alignment, &child_content_box.y, &child_content_box.height);
		dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
	}
}

/*
 *
 * STACK
 *
 */

/* Arranges content for a horizontal (LEFT_TO_RIGHT or RIGHT_TO_LEFT) stack. */
static void
dt_layout_Stack_horiz_arrange_content(
	struct dt_layout_Stack const* const stack,
	struct dt_layout_Bounds* const io_bounds
) {
	float content_width = 0.f;
	float content_height = 0.f;
	float left_padding = 0.f;
	for (struct dt_layout_StackChild const* child = stack->first_child; child != NULL; child = child->next_child)
	{
		content_width = fmaxf(left_padding, child->margin.left) + child->bounds.computed_width;
		left_padding = child->margin.right;

		float const child_height = child->margin.top + child->bounds.computed_height + child->margin.bottom;
		content_height = fmaxf(content_height, child_height);
	}

	io_bounds->computed_width = content_width + left_padding;
	io_bounds->computed_height = content_height;
}

/* Arranges content for a vertical (TOP_TO_BOTTOM or BOTTOM_TO_TOP) stack. */
static void
dt_layout_Stack_vert_arrange_content(
	struct dt_layout_Stack const* const stack,
	struct dt_layout_Bounds* const io_bounds
) {
	float content_width = 0.f;
	float content_height = 0.f;
	float top_padding = 0.f;
	for (struct dt_layout_StackChild const* child = stack->first_child; child != NULL; child = child->next_child)
	{
		content_height = fmaxf(top_padding, child->margin.top) + child->bounds.computed_height;
		top_padding = child->margin.bottom;

		float const child_width = child->margin.left + child->bounds.computed_width + child->margin.right;
		content_width = fmaxf(content_width, child_width);
	}

	io_bounds->computed_height = content_height + top_padding;
	io_bounds->computed_width = content_width;
}

void
dt_layout_Stack_arrange_content(
	struct dt_layout_Stack const* const layout_stack,
	struct dt_layout_Bounds* const io_bounds
) {
	switch (layout_stack->stack_direction)
	{
	case DT_STACK_LEFT_TO_RIGHT:
	case DT_STACK_RIGHT_TO_LEFT:
		dt_layout_Stack_horiz_arrange_content(layout_stack, io_bounds);
		break;

	case DT_STACK_TOP_TO_BOTTOM:
	case DT_STACK_BOTTOM_TO_TOP:
		dt_layout_Stack_vert_arrange_content(layout_stack, io_bounds);
		break;

	default:
		assert(false /* Unkown dt_layout_Stack orientation */);
	}
}

/* Generates display commands for a LEFT_TO_RIGHT stack */
static void
dt_layout_Stack_ltr_generate_commands(
	struct dt_layout_Stack const* const stack,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox content_box
) {
	if (!stack->first_child)
	{
		return;
	}

	// Generate commands for children (up until last child)
	float left_padding = 0.f;
	struct dt_layout_StackChild const* child;
	for (child = stack->first_child; child->next_child != NULL; child = child->next_child)
	{
		struct dt_layout_ContentBox child_content_box = content_box;

		// Align horizontally to the left (compact)
		dt_layout_align_top_compact(
			child->bounds.min_width,
			child->bounds.computed_width,
			fmaxf(left_padding, child->margin.left),
			child->margin.right,
			&child_content_box.x,
			&child_content_box.width,
			&content_box.x,
			&content_box.width);

		// Align vertically as child desires (stretch)
		dt_layout_align_y(
			child->bounds.max_height,
			child->margin.top,
			child->margin.bottom,
			child->cross_alignment,
			&child_content_box.y,
			&child_content_box.height);

		dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
		left_padding = child->margin.right;
	}

	// Generate commands for last child
	struct dt_layout_ContentBox child_content_box = content_box;
	left_padding = fmaxf(left_padding, child->margin.left);

	// Align vertically as child desires (stretch)
	dt_layout_align_y(
		child->bounds.max_height,
		child->margin.top,
		child->margin.bottom,
		child->cross_alignment,
		&child_content_box.y,
		&child_content_box.height);

	if (stack->last_child_fill)
	{
		dt_layout_align_top_stretch(
			child->bounds.min_width,
			child->bounds.max_width,
			child->bounds.computed_width,
			left_padding,
			child->margin.right,
			&child_content_box.x,
			&child_content_box.width);
	}
	else
	{
		dt_layout_align_top_compact(
			child->bounds.min_width,
			child->bounds.computed_width,
			left_padding,
			child->margin.right,
			&child_content_box.x,
			&child_content_box.width,
			&content_box.x,
			&content_box.width);
	}

	dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
}

/* Generates display commands for a RIGHT_TO_LEFT stack */
static void
dt_layout_Stack_rtl_generate_commands(
	struct dt_layout_Stack const* const stack,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox content_box
) {
	if (!stack->first_child)
	{
		return;
	}

	// Generate commands for children (up until last child)
	float right_padding = 0.f;
	struct dt_layout_StackChild const* child;
	for (child = stack->first_child; child->next_child != NULL; child = child->next_child)
	{
		struct dt_layout_ContentBox child_content_box = content_box;

		// Align horizontally to the right (compact)
		dt_layout_align_bottom_compact(
			child->bounds.min_width,
			child->bounds.computed_width,
			child->margin.left,
			fmaxf(right_padding, child->margin.right),
			&child_content_box.x,
			&child_content_box.width,
			&content_box.width);

		// Align vertically as child desires (stretch)
		dt_layout_align_y(
			child->bounds.max_height,
			child->margin.top,
			child->margin.bottom,
			child->cross_alignment,
			&child_content_box.y,
			&child_content_box.height);

		dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
		right_padding = child->margin.left;
	}

	// Generate commands for last child
	struct dt_layout_ContentBox child_content_box = content_box;
	right_padding = fmaxf(right_padding, child->margin.right);

	// Align vertically as child desires (stretch)
	dt_layout_align_y(
		child->bounds.max_height,
		child->margin.top,
		child->margin.bottom,
		child->cross_alignment,
		&child_content_box.y,
		&child_content_box.height);

	if (stack->last_child_fill)
	{
		dt_layout_align_bottom_stretch(
			child->bounds.min_width,
			child->bounds.max_width,
			child->bounds.computed_width,
			child->margin.left,
			right_padding,
			&child_content_box.x,
			&child_content_box.width);
	}
	else
	{
		dt_layout_align_bottom_compact(
			child->bounds.min_width,
			child->bounds.computed_width,
			child->margin.left,
			right_padding,
			&child_content_box.x,
			&child_content_box.width,
			&content_box.width);
	}

	dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
}

/* Generates display commands for a TOP_TO_BOTTOM stack */
static void
dt_layout_Stack_ttb_generate_commands(
	struct dt_layout_Stack const* const stack,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox content_box
) {
	if (!stack->first_child)
	{
		return;
	}

	// Generate commands for children (up until last child)
	float top_padding = 0.f;
	struct dt_layout_StackChild const* child;
	for (child = stack->first_child; child->next_child != NULL; child = child->next_child)
	{
		struct dt_layout_ContentBox child_content_box = content_box;

		// Align vertically to the top (compact)
		dt_layout_align_top_compact(
			child->bounds.min_height,
			child->bounds.computed_height,
			fmaxf(top_padding, child->margin.top),
			child->margin.bottom,
			&child_content_box.y,
			&child_content_box.height,
			&content_box.y,
			&content_box.height);

		// Align horizontally as child desires (stretch)
		dt_layout_align_x(
			child->bounds.max_width,
			child->margin.left,
			child->margin.right,
			child->cross_alignment,
			&child_content_box.x,
			&child_content_box.width);

		dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
		top_padding = child->margin.bottom;
	}

	// Generate commands for last child
	struct dt_layout_ContentBox child_content_box = content_box;
	top_padding = fmaxf(top_padding, child->margin.top);

	// Align horizontally as child desires (stretch)
	dt_layout_align_x(
		child->bounds.max_width,
		child->margin.left,
		child->margin.right,
		child->cross_alignment,
		&child_content_box.x,
		&child_content_box.width);

	if (stack->last_child_fill)
	{
		dt_layout_align_top_stretch(
			child->bounds.min_height,
			child->bounds.max_height,
			child->bounds.computed_height,
			top_padding,
			child->margin.bottom,
			&child_content_box.y,
			&child_content_box.height);
	}
	else
	{
		dt_layout_align_top_compact(
			child->bounds.min_height,
			child->bounds.computed_height,
			top_padding,
			child->margin.bottom,
			&child_content_box.y,
			&child_content_box.height,
			&content_box.y,
			&content_box.height);
	}

	dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
}

/* Generates display commands for a BOTTOM_TO_TOP stack */
static void
dt_layout_Stack_btt_generate_commands(
	struct dt_layout_Stack const* const stack,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox content_box
) {
	if (!stack->first_child)
	{
		return;
	}

	// Generate commands for children (up until last child)
	float bottom_padding = 0.f;
	struct dt_layout_StackChild const* child;
	for (child = stack->first_child; child->next_child != NULL; child = child->next_child)
	{
		struct dt_layout_ContentBox child_content_box = content_box;

		// Align vertically to the bottom (compact)
		dt_layout_align_bottom_compact(
			child->bounds.min_height,
			child->bounds.computed_height,
			child->margin.top,
			fmaxf(bottom_padding, child->margin.bottom),
			&child_content_box.y,
			&child_content_box.height,
			&content_box.height);

		// Align horizontally as child desires (stretch)
		dt_layout_align_x(
			child->bounds.max_width,
			child->margin.left,
			child->margin.right,
			child->cross_alignment,
			&child_content_box.x,
			&child_content_box.width);

		dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
		bottom_padding = child->margin.top;
	}

	// Generate commands for last child
	struct dt_layout_ContentBox child_content_box = content_box;
	bottom_padding = fmaxf(bottom_padding, child->margin.bottom);

	// Align horizontally as child desires (stretch)
	dt_layout_align_x(
		child->bounds.max_width,
		child->margin.left,
		child->margin.right,
		child->cross_alignment,
		&child_content_box.x,
		&child_content_box.width);

	if (stack->last_child_fill)
	{
		dt_layout_align_bottom_stretch(
			child->bounds.min_height,
			child->bounds.max_height,
			child->bounds.computed_height,
			child->margin.top,
			bottom_padding,
			&child_content_box.y,
			&child_content_box.height);
	}
	else
	{
		dt_layout_align_bottom_compact(
			child->bounds.min_height,
			child->bounds.computed_height,
			child->margin.top,
			bottom_padding,
			&child_content_box.y,
			&child_content_box.height,
			&content_box.height);
	}

	dt_layout_Element_generate_commands(child->element, command_allocator, child_content_box);
}

void
dt_layout_Stack_generate_commands(
	struct dt_layout_Stack const* const stack,
	struct dt_Allocator* const command_allocator,
	struct dt_layout_ContentBox const content_box
) {
	switch (stack->stack_direction)
	{
	case DT_STACK_LEFT_TO_RIGHT:
		dt_layout_Stack_ltr_generate_commands(stack, command_allocator, content_box);
		break;

	case DT_STACK_RIGHT_TO_LEFT:
		dt_layout_Stack_rtl_generate_commands(stack, command_allocator, content_box);
		break;

	case DT_STACK_TOP_TO_BOTTOM:
		dt_layout_Stack_ttb_generate_commands(stack, command_allocator, content_box);
		break;

	case DT_STACK_BOTTOM_TO_TOP:
		dt_layout_Stack_btt_generate_commands(stack, command_allocator, content_box);
		break;

	default:
		assert(false /* Unknown dt_layout_Stack orientation */);
	}
}

/*
 *
 * WINDOW
 *
 */

void
dt_layout_Window_arrange_content(
	struct dt_layout_Window* const window
) {
	dt_layout_Box_arrange_content(window->content, &window->content_bounds);
}

DT_FUNC void
dt_layout_Window_generate_commands(
	struct dt_layout_Window const* const window,
	struct dt_Allocator* const command_allocator,
	float const width,
	float const height
) {
	dt_layout_generate_rect(command_allocator, 0.f, 0.f, width, height, window->background_color);

	struct dt_layout_ContentBox content_box;
	content_box.x = 0.f;
	content_box.y = 0.f;
	content_box.width = width;
	content_box.height = height;
	dt_layout_Box_generate_commands(window->content, command_allocator, content_box);
}

