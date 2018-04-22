// main.c

#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "../include/Delta/template.h"
#include "../include/Delta/render_command.h"
#include "../private/serializer.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

int main(
	int argc,
	char** argv
) {
	char layout_buffer[4096];
	char command_buffer[4096];
	struct dt_Allocator layout_allocator;
	layout_allocator.buffer = layout_buffer;
	layout_allocator.capacity = sizeof(layout_buffer);
	layout_allocator.offset = 0;
	struct dt_Allocator command_allocator;
	command_allocator.buffer = command_buffer;
	command_allocator.capacity = sizeof(command_buffer);
	command_allocator.offset = 0;

	// Load UI
	struct dt_template_Window const* ui_root = dt_load_ui("test.xml");

    // Initialize GLFW
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    GLFWwindow* const window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "delta", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

	// Start the game loop
	while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();

        int width, height;
        glfwGetWindowSize(window, &width, &height);

		// Generate layout
		struct dt_layout_Window layout_window;
		layout_allocator.offset = 0;
		dt_template_Window_generate_layout(ui_root, NULL, &layout_allocator, &layout_window);

        // Generate commands
		command_allocator.offset = 0;
		dt_layout_Window_generate_commands(&layout_window, &command_allocator, (float)width, (float)height);

		// Consume rendering commands
		struct dt_RenderCommand const* const render_commands = (struct dt_RenderCommand const*)command_allocator.buffer;
		size_t const num_commands = command_allocator.offset / sizeof(struct dt_RenderCommand);

		// Clear screen and draw
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
		for (size_t i = 0; i < num_commands; ++i)
		{
            GLubyte const red = dt_Color_get_red(render_commands[i].color);
            GLubyte const green = dt_Color_get_green(render_commands[i].color);
            GLubyte const blue = dt_Color_get_blue(render_commands[i].color);
            GLubyte const alpha = dt_Color_get_alpha(render_commands[i].color);
            glColor4ub(red, green, blue, alpha);

            glVertex2f(render_commands[i].coords[0] / width * 2 - 1, render_commands[i].coords[1] / height * 2 - 1);
            glVertex2f(render_commands[i].coords[2] / width * 2 - 1, render_commands[i].coords[3] / height * 2 - 1);
            glVertex2f(render_commands[i].coords[4] / width * 2 - 1, render_commands[i].coords[5] / height * 2 - 1);
		}
        glEnd();

        glfwSwapBuffers(window);
	}

    glfwTerminate();
}
