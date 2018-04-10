// main.cpp

#include <stdio.h>
#include <chrono>
#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include "../include/Delta/template.h"
#include "../include/Delta/render_command.h"
#include "../private/serializer.h"

static constexpr float WINDOW_WIDTH = 800.f;
static constexpr float WINDOW_HEIGHT = 600.f;

int main(
	int argc,
	char** argv
) {
	char layout_buffer[4096];
	char command_buffer[4096];
	dt_Allocator layout_allocator;
	layout_allocator.buffer = layout_buffer;
	layout_allocator.capacity = sizeof(layout_buffer);
	layout_allocator.offset = 0;
	dt_Allocator command_allocator;
	command_allocator.buffer = command_buffer;
	command_allocator.capacity = sizeof(command_buffer);
	command_allocator.offset = 0;

	// Load UI
	auto* ui_root = dt::load_ui("test.xml");

	// Create an SFML window
	sf::RenderWindow window(sf::VideoMode((unsigned)WINDOW_WIDTH, (unsigned)WINDOW_HEIGHT), "SFML window");

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// Generate commands
		layout_allocator.offset = 0;
		command_allocator.offset = 0;
		auto const window_size = window.getSize();
		float const width_ratio = WINDOW_WIDTH / window_size.x;
		float const height_ratio = WINDOW_HEIGHT / window_size.y;

		// Generate layout
		dt_layout_Window layout_window;
		dt_template_Window_generate_layout(ui_root, NULL, &layout_allocator, &layout_window);
		auto const gen_command_start = std::chrono::high_resolution_clock::now();
		dt_layout_Window_generate_commands(&layout_window, &command_allocator, (float)window_size.x, (float)window_size.y);
		auto const gen_command_duration = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - gen_command_start).count();
		printf("Command generation time: %f ms\n", gen_command_duration/1'000'000);

		// Clear screen and draw
		window.clear();

		// Consume rendering commands
		dt_RenderCommand const* const render_commands = (dt_RenderCommand const*)command_allocator.buffer;
		size_t const num_commands = command_allocator.offset / sizeof(dt_RenderCommand);
		sf::VertexArray vert_array{ sf::PrimitiveType::Triangles, num_commands * 3 };
		for (size_t i = 0; i < num_commands; ++i)
		{
			// Generate vertices for the render command
			sf::Vertex vert;
			vert.color = sf::Color{ render_commands[i].color };

			vert.position = sf::Vector2f{ render_commands[i].coords[0] * width_ratio, render_commands[i].coords[1] * height_ratio };
			vert_array.append(vert);

			vert.position = sf::Vector2f{ render_commands[i].coords[2] * width_ratio, render_commands[i].coords[3] * height_ratio };
			vert_array.append(vert);

			vert.position = sf::Vector2f{ render_commands[i].coords[4] * width_ratio, render_commands[i].coords[5] * height_ratio };
			vert_array.append(vert);
		}

		// Draw render commands
		window.draw(vert_array);

		window.display();
	}
}
