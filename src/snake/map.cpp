#include "snake/map.hpp"

#include <vector>
#include <random>

#include <glibmm/main.h>

#include "snake/properties.hpp"

namespace snake
{
	/// A pseudo-random number generator for assigning random positions.
	std::minstd_rand rand(std::random_device{}());

	map::map(Gtk::Window& parent) :
		parent(parent), states({}), snake_direction(direction_up)
	{
		// Set callback for drawing
		set_draw_func(sigc::mem_fun(*this, &map::on_draw));
	}

	void map::start_game()
	{
		static std::uniform_int_distribution<std::size_t> distr(
			0, properties::map_height - 1
		);
		static std::uniform_int_distribution<std::size_t> distc(
			0, properties::map_width - 1
		);

		// Initialise the game
		states.fill({});
		snake_occupancy.clear();

		// Set initial positions of the snake and the first apple
		auto snake_start = std::make_pair(distr(rand), distc(rand));
		auto apple_start = std::make_pair(distr(rand), distc(rand));

		// Keep setting apple's position until it is different with the snake
		for(;
			apple_start == snake_start;
			apple_start = std::make_pair(distr(rand), distc(rand))
		) {}

		// Set the snake's initial direction based on its initial position
		struct {
			std::size_t north;
			std::size_t south;
			std::size_t west;
			std::size_t east;
		} direction_factor {
			properties::map_height - 1 - snake_start.first,
			snake_start.first,
			properties::map_width - 1 - snake_start.second,
			snake_start.second
		};
		auto factor = direction_factor.north;

		snake_direction = direction_up;
		if(direction_factor.south < factor)
		{
			snake_direction = direction_down;
			factor = direction_factor.south;
		}
		if(direction_factor.west < factor)
		{
			snake_direction = direction_left;
			factor = direction_factor.west;
		}
		if(direction_factor.east < factor)
		{
			snake_direction = direction_right;
			factor = direction_factor.east;
		}

		// Finish initialisation and schedule draw
		snake_occupancy.push_back(snake_start);
		states[snake_start.first][snake_start.second] = state_snake;
		states[apple_start.first][apple_start.second] = state_apple;
		queue_draw();

		// Start the game
		static int timeout_count = 0;
		auto slot = sigc::bind(sigc::mem_fun(*this, &map::on_timeout), timeout_count++);
		Glib::signal_timeout().connect(slot, properties::tickrate);
	}

	void map::set_direction(snake::direction new_direction)
	{
		// Set the new direction
		snake_direction = new_direction;
	}

	void map::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
	{
		// Set border width and colour
		cr->set_line_width(static_cast<double>(properties::border_width));
		cr->set_source_rgb(0.0, 0.0, 0.0);

		// Draw vertical borders
		for(
			int i = 1;
			i <= properties::content_width;
			i += (properties::border_width + properties::grid_size)
		)
		{
			cr->move_to(
				static_cast<double>(i),
				static_cast<double>(properties::border_width)
			);
			cr->line_to(
				static_cast<double>(i),
				static_cast<double>(properties::content_height)
			);
		}

		// Draw horizontal borders
		for(
			int i = 1;
			i <= properties::content_height;
			i += (properties::border_width + properties::grid_size)
		)
		{
			cr->move_to(
				static_cast<double>(properties::border_width),
				static_cast<double>(i)
			);
			cr->line_to(
				static_cast<double>(properties::content_width),
				static_cast<double>(i)
			);
		}

		// Finish drawing
		cr->stroke();

		// Fill spaces in the grid
		{
			int x = properties::border_width + 1;
			int y = properties::border_width + 1;

			for(const auto& row : states)
			{
				for(state column : row)
				{
					// Choose colour depending on the state:
					switch(column)
					{
					// Use green for the snake
					case state_snake:
						cr->set_source_rgb(0.0, 1.0, 0.0);
						break;

					// Use red for the apple
					[[unlikely]] case state_apple:
						cr->set_source_rgb(1.0, 0.0, 0.0);
						break;

					// Use white for an empty space
					[[likely]] case state_blank:
						[[fallthrough]];
					default:
						cr->set_source_rgb(1.0, 1.0, 1.0);
						break;
					}

					// Draw the rectangle
					cr->rectangle(
						static_cast<double>(x),
						static_cast<double>(y),
						static_cast<double>(properties::grid_size),
						static_cast<double>(properties::grid_size)
					);
					cr->fill();

					x += (properties::grid_size + properties::border_width);
				}
				x = 2;
				y += (properties::grid_size + properties::border_width);
			}
		}
	}

	bool map::on_timeout(int timeout_number)
	{
		auto fail = [this]() { this->show_dialog(); return false; };

		// Check if the snake reaches out of bounds
		auto next = snake_occupancy.back();
		switch(snake_direction)
		{
		case direction_up:
			if(next.first == 0) [[unlikely]] return fail();
			--next.first;
			break;
		case direction_down:
			if(++next.first >= properties::map_height) [[unlikely]] return fail();
			break;
		case direction_left:
			if(next.second == 0) [[unlikely]] return fail();
			--next.second;
			break;
		case direction_right:
			if(++next.second >= properties::map_width) [[unlikely]] return fail();
			break;
		}

		// Check the next position
		if(states[next.first][next.second] == state_blank) [[likely]]
		{
			// Get rid of the snake's tail upon encountering an empty space
			auto tail = snake_occupancy.front();
			states[tail.first][tail.second] = state_blank;
			snake_occupancy.pop_front();
		}
		else if(states[next.first][next.second] == state_snake)
		{
			// Finish the game for biting its own tail
			return fail();
		}
		else [[unlikely]]
		{
			// Prepare for creation of a new apple
			static std::vector<std::pair<std::size_t, std::size_t>> blanks;
			blanks.reserve(properties::map_width * properties::map_height);
			blanks.clear();

			// Choose only empty spaces for inclusion
			for(std::size_t i = 0; i < states.size(); ++i)
			{
				for(std::size_t j = 0; j < states[i].size(); ++j)
				{
					if(states[i][j] == state_blank)
						blanks.emplace_back(i, j);
				}
			}

			if(!blanks.empty())
			{
				// Initialise a new distribution
				std::uniform_int_distribution<decltype(blanks)::size_type> distribution(
					0, blanks.size() - 1
				);

				// Randomly assign a new position for the next apple
				auto selected = blanks[distribution(rand)];
				states[selected.first][selected.second] = state_apple;
			}
		}

		// Add the new head
		snake_occupancy.push_back(next);
		states[next.first][next.second] = state_snake;

		// Draw again to reflect changes
		queue_draw();

		return true;
	}

	void map::show_dialog()
	{
		// Create and show a new dialog
		auto dialog = Gtk::AlertDialog::create(Glib::ustring("Game over"));
		dialog->choose(
			parent,
			sigc::bind(sigc::mem_fun(*this, &map::on_choose))
		);
	}

	void map::on_choose(Glib::RefPtr<Gio::AsyncResult>&)
	{
		start_game();
	}
}
