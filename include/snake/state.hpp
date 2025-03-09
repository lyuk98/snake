#ifndef SNAKE_STATE_HPP_INCLUDED
#define SNAKE_STATE_HPP_INCLUDED

namespace snake
{
	/// Possible states for each grid in the game map.
	enum class state : unsigned char
	{
		blank,
		snake,
		apple
	};

	inline constexpr auto state_blank = state::blank;
	inline constexpr auto state_snake = state::snake;
	inline constexpr auto state_apple = state::apple;
}

#endif
