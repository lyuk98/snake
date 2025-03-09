#ifndef SNAKE_WINDOW_HPP_INCLUDED
#define SNAKE_WINDOW_HPP_INCLUDED

#include <gtkmm/window.h>

#include "snake/map.hpp"

namespace snake
{
	/// The game window.
	class window : public Gtk::Window
	{
	public:
		/// Constructs a new window.
		window();

	protected:
		/// The map area.
		snake::map map_area;

		/// A callback function called upon key presses.
		bool on_movement_key_pressed(guint key, guint, Gdk::ModifierType state);
	};
}

#endif
