#pragma once
#ifndef PLAYER_HPP
#define PLAYER_HPP


#include <forward_list>
#include "coords.hpp"


class player {
	// TODO: update position in a function

public:
	coords position{};
	std::forward_list<coords> prev_positions;
	int health = 100;
};


#endif  // PLAYER_HPP
