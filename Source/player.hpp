#pragma once
#ifndef PLAYER_HPP
#define PLAYER_HPP


#include <deque>
#include "coords.hpp"


class player {
	// TODO: update position in a function

public:
	coords position{};
	std::deque<coords> prev_positions;
	int health = 100;
};


#endif  // PLAYER_HPP
