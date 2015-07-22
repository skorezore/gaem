#include "game_screen.hpp"
#include "player.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <conio.h>


using namespace std;
using namespace std::chrono;


bool handle_player_movements(game_screen & screen, player & player) {
  if(kbhit()) {
    char key      = tolower(getch());
    coords newpos = player.position;

    if(key == 'w' && player.position.x > 2 && screen[player.position.below()] != game_screen::filler && screen[player.position.above()] == game_screen::filler &&
       screen[player.position.above(2)] == game_screen::filler)
      newpos.y -= 2;

    if(key == 'a' && player.position.x > 0)
      --newpos.x;

    if(key == 's' && player.position.y < screen.size.y)
      ++newpos.y;

    if(key == 'd' && player.position.x < screen.size.x)
      ++newpos.x;

    player.prev_positions.emplace_front(move(player.position));
    player.position = move(newpos);

    if(key == 'q')  // Sneaking in that close key
      return true;
  }
  return false;
}

void gravity(game_screen & screen, player & player) {
  if(player.position.y < screen.size.y && screen[player.position.above()] == game_screen::filler && screen[player.position.below()] == game_screen::filler) {
    player.prev_positions.emplace_front(player.position);
    ++player.position.y;
  }
}

void loop() {
  static const auto time_between_frames = 100ms;

  game_screen screen({16, 16});
  screen.reset();
  screen[{4, 13}]  = '=';
  screen[{5, 13}]  = '=';
  screen[{6, 13}]  = '=';
  screen[{7, 13}]  = '=';
  screen[{8, 13}]  = '=';
  screen[{10, 14}] = '=';
  screen[{11, 14}] = '=';
  // Look at that fancy hardcoded screen ^
  player player;
  bool do_gravity = false;

  while(true) {
    if((do_gravity ^= true) % 2 == 0)
      gravity(screen, player);
    this_thread::sleep_for(time_between_frames);
    system("cls");
    for(auto & pos : player.prev_positions)
      screen[pos] = game_screen::filler;
    player.prev_positions.clear();
    screen[player.position] = 'X';
    screen.draw();
    cout << "^^^^^^^^^^^^^^^^^^^\n\n"  // Photo-realistic spikes, I know.
            "Use WASD for movement\n"
            "Press Q to quit\n\n"
            "X: "
         << player.position.x << " Y: " << (-player.position.y + screen.size.y) << "\nWatch out for the spikes below!" << endl;

    if(handle_player_movements(screen, player))
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
