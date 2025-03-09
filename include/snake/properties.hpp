#ifndef SNAKE_PROPERTIES_HPP_INCLUDED
#define SNAKE_PROPERTIES_HPP_INCLUDED

namespace snake
{
	namespace properties
	{
		/// Width and height of a single grid in pixels.
		inline constexpr int grid_size = 20;

		/// Number of horizontal grids in the map.
		inline constexpr int map_width = 20;
		/// Number of vertical grids in the map.
		inline constexpr int map_height = 20;

		/// Map width.
		inline constexpr auto content_width =
			properties::map_width * properties::grid_size + properties::map_width + 1;
		/// Map height.
		inline constexpr auto content_height =
			properties::map_height * properties::grid_size + properties::map_height + 1;

		/// Border width.
		inline constexpr int border_width = 1;

		/// Time before advancing state in milliseconds.
		inline constexpr int tickrate = 100;
	}
}

#endif
