#ifndef SNAKE_MAP_HPP_INCLUDED
#define SNAKE_MAP_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <array>
#include <deque>
#include <atomic>

#include <cairomm/context.h>
#include <giomm/asyncresult.h>
#include <gtkmm/alertdialog.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

#include "snake/direction.hpp"
#include "snake/properties.hpp"
#include "snake/state.hpp"

namespace snake
{
	/// Map area of the game.
	class map : public Gtk::DrawingArea
	{
	public:
		/// Constructs a new map.
		map(Gtk::Window& parent);

		/// Starts a new game.
		void start_game();

		/// Sets the snake's direction.
		void set_direction(snake::direction new_direction);

	protected:
		/// A callback function called upon drawing.
		void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

		/// A callback function called upon reaching timeout.
		bool on_timeout(int timeout_number);

		/// Shows a game-over dialog.
		void show_dialog();

		/// A callback function called when the dialog is dismissed.
		void on_choose(Glib::RefPtr<Gio::AsyncResult>&);

	private:
		/// Parent of this widget.
		Gtk::Window& parent;

		// State of the game area.
		std::array<
			std::array<
				state,
				properties::map_width
			>,
			properties::map_height
		> states;

		// Current direction of the snake.
		std::atomic<snake::direction> snake_direction;

		// The list of coordinates the snake is occupying.
		std::deque<std::pair<std::size_t, std::size_t>> snake_occupancy;
	};
}

#endif
