#include "game_screen.hpp"
#include <iostream>
#include <sstream>


using namespace std;


const char game_screen::filler = ' ';


game_screen::game_screen(const coords & sz) : size(sz) {
  coords xy{};
  for(; xy.x < size.x; ++xy.x) {
    for(; xy.y < size.y; ++xy.y)
      map.emplace(xy, filler);
    xy.y = 0;
  }
}

void game_screen::draw() {
  stringstream frame;
  for(auto itr = map.begin(); itr != map.end(); ++itr) {
    for(int i = 1; i < size.x; ++i)
      frame << itr++->second;
    frame << '\n';
  }
  cout << frame.str();
}

void game_screen::reset() {
  for(auto & pr : map)
    pr.second = filler;
}

reference_proxy<char> game_screen::operator[](const coords & xy) {
  try {
    return map.at(xy);
  } catch(...) {
    return filler;
  }
}
