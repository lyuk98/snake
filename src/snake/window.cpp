#include "snake/window.hpp"

#include <gtkmm/eventcontrollerkey.h>

#include "snake/direction.hpp"
#include "snake/properties.hpp"

namespace snake
{
	window::window() : map_area(*this)
	{
		using namespace properties;

		// Set window title and size
		set_title("Snake");
		set_default_size(
			content_width + 32,
			content_height + 32
		);

		// Set map area
		map_area.set_content_width(content_width + 2);
		map_area.set_content_height(content_height + 2);
		map_area.set_valign(Gtk::Align::CENTER);
		map_area.set_halign(Gtk::Align::CENTER);
		set_child(map_area);

		// Register callback for key presses
		auto controller = Gtk::EventControllerKey::create();
		controller->signal_key_pressed(
		).connect(
			sigc::mem_fun(*this, &window::on_movement_key_pressed),
			false
		);
		add_controller(controller);

		// Start the game
		map_area.start_game();
	}

	bool window::on_movement_key_pressed(guint key, guint, Gdk::ModifierType state)
	{
		if(key == GDK_KEY_Up)
		{
			map_area.set_direction(direction_up);
			return true;
		}
		else if(key == GDK_KEY_Down)
		{
			map_area.set_direction(direction_down);
			return true;
		}
		else if(key == GDK_KEY_Left)
		{
			map_area.set_direction(direction_left);
			return true;
		}
		else if(key == GDK_KEY_Right)
		{
			map_area.set_direction(direction_right);
			return true;
		}

		// Return false for other kinds of key presses
		return false;
	}
}
