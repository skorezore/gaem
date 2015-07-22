#include <iostream>
#include <array>
#include <algorithm>
#include <chrono>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <map>
#include <conio.h>

#include "game_screen.hpp"
#include "player.hpp"


using namespace std;
using namespace std::chrono;


bool handle_player_movements(game_screen & map, player & player) {
  if(kbhit()) {
    char key      = tolower(getch());
    coords newpos = player.position;

    if(key == 'w' && player.position.x > 2 && map[player.position.below()] != game_screen::filler && map[player.position.above()] == game_screen::filler &&
       map[player.position.above(2)] == game_screen::filler)
      newpos.y -= 2;

    if(key == 'a' && player.position.x > 0)
      --newpos.x;

    if(key == 's' && player.position.y < 15)
      ++newpos.y;

    if(key == 'd' && player.position.x < 15)
      ++newpos.x;

    player.prev_positions.emplace_front(move(player.position));
    player.position = move(newpos);

    if(key == 'q')  // Sneaking in that close key
      return true;
  }
  return false;
}

void gravity(game_screen & map, player & player) {
  if(player.position.y < 15 && map[player.position.above()] == game_screen::filler && map[player.position.below()] == game_screen::filler) {
    player.prev_positions.emplace_front(player.position);
    ++player.position.y;
  }
}

void loop() {
  static const auto time_between_frames = 100ms;

  game_screen map({16, 16});
  map.reset();
  map[{4, 13}]  = '=';
  map[{5, 13}]  = '=';
  map[{6, 13}]  = '=';
  map[{7, 13}]  = '=';
  map[{8, 13}]  = '=';
  map[{10, 14}] = '=';
  map[{11, 14}] = '=';
  // Look at that fancy hardcoded map ^
  player player;
  bool do_gravity = false;

  while(true) {
    if((do_gravity ^= true) % 2 == 0)
      gravity(map, player);
    this_thread::sleep_for(time_between_frames);
    system("cls");
    for(auto & pos : player.prev_positions)
      map[pos] = game_screen::filler;
    player.prev_positions.clear();
    map[player.position] = 'X';
    map.draw();
    cout << "^^^^^^^^^^^^^^^^^^^\n\n"  // Photo-realistic spikes, I know.
            "Use WASD for movement\n"
            "Press Q to quit\n\n"
            "X: "
         << player.position.x << " Y: " << (-player.position.y + map.size.y) << "\nWatch out for the spikes below!" << endl;

    if(handle_player_movements(map, player))
      break;

    if(player.position.y > 14) {
      cout << "You fell to your death. Game over!" << endl;
      break;
    }
  }
}

int main() {
  loop();
}
