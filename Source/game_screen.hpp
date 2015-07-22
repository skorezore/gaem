#pragma once
#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP


#include "coords.hpp"
#include "reference_proxy.hpp"
#include <map>


class game_screen {
public:
  static const char filler;

  coords size;
  std::map<coords, char> map;

  game_screen(const coords & sz);

  void draw();
  void reset();

  reference_proxy<char> operator[](const coords & xy);
};


#endif  // GAME_SCREEN_HPP
