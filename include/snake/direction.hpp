#ifndef SNAKE_DIRECTION_HPP_INCLUDED
#define SNAKE_DIRECTION_HPP_INCLUDED

namespace snake
{
	/// A representation of the direction the snake is moving.
	enum class direction : unsigned char
	{
		up,
		down,
		left,
		right
	};

	inline constexpr auto direction_up = direction::up;
	inline constexpr auto direction_down = direction::down;
	inline constexpr auto direction_left = direction::left;
	inline constexpr auto direction_right = direction::right;
}

#endif
