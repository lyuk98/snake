#include <gtkmm/application.h>

#include "snake/window.hpp"

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create("com.lyuk98.snake");
	return app->make_window_and_run<snake::window>(argc, argv);
}
