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
#include <iomanip>
#include <conio.h>


using namespace std;
using namespace std::chrono;


struct coords {
  int x;
  int y;

  coords below(int factor = 1) const {
    return {x, y + factor};
  }

  coords above(int factor = 1) const {
    return {x, y - factor};
  }
};

ostream & operator<<(ostream & strm, const coords & xy) {
  strm << "{x=" << xy.x << ",y=" << xy.y << '}';
  return strm;
}

bool operator<(const coords & lhs, const coords & rhs) {
  return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}

template <class T>
class proxy {
  friend ostream & operator<<(ostream & strm, const proxy & px) {
    strm << '[' << px.mem << ']';
    return strm;
  }

private:
  T & mem;
  bool share;

public:
  proxy(T & ref, bool shr = true) : mem(ref), share(shr) {}
  proxy(const T & ref) : proxy(const_cast<T &>(ref), false) {}

  proxy & operator=(const T & val) {
    if(share)
      mem = val;
    return *this;
  }

  bool operator==(const T & val) {
    return mem == val;
  }

  bool operator!=(const T & val) {
    return mem != val;
  }
};

template <class T>
bool operator<(const proxy<T> & lhs, const proxy<T> & rhs) {
  return lhs.mem < rhs.mem;
}

template <class T>
bool operator>(const proxy<T> & lhs, const proxy<T> & rhs) {
  return lhs.mem > rhs.mem;
}

class Map {
public:
  static const char filler;

  coords size;
  map<coords, char> map;

  Map(const coords & sz) : size(sz) {
    coords xy{};
    for(; xy.x < size.x; ++xy.x) {
      for(; xy.y < size.y; ++xy.y)
        map.emplace(xy, filler);
      xy.y = 0;
    }
  }

  void draw() {
    stringstream frame;
    for(auto itr = map.begin(); itr != map.end(); ++itr) {
      for(int i = 1; i < size.x; ++i)
        frame << itr++->second;
      frame << '\n';
    }
    cout << frame.str();
  }

  void reset() {
    for(auto & pr : map)
      pr.second = filler;
  }

  proxy<char> operator[](const coords & xy) {
    try {
      return map.at(xy);
    } catch(...) {
      return filler;
    }
  }
};

const char Map::filler = ' ';

class Player {
public:
  coords position{};
  vector<coords> prev_positions;
  int health = 100;
};

bool handle_player_movements(Map & map, Player & player) {
  if(kbhit()) {
    char key      = tolower(getch());
    coords newpos = player.position;

    if(key == 'w' && player.position.x > 2 && map[player.position.below()] != Map::filler && map[player.position.above()] == Map::filler &&
       map[player.position.above(2)] == Map::filler)
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

void gravity(Map & map, Player & player) {
  if(player.position.y < 15 && map[player.position.above()] == Map::filler && map[player.position.below()] == Map::filler) {
    player.prev_positions.emplace_front(player.position);
    ++player.position.y;
  }
}

void Loop() {
  static const auto time_between_frames = 100ms;

  Map map({16, 16});
  map.reset();
  map[{4, 13}]  = '=';
  map[{5, 13}]  = '=';
  map[{6, 13}]  = '=';
  map[{7, 13}]  = '=';
  map[{8, 13}]  = '=';
  map[{10, 14}] = '=';
  map[{11, 14}] = '=';
  // Look at that fancy hardcoded map ^
  Player player;
  bool do_gravity = false;

  while(true) {
    if((do_gravity ^= true) % 2 == 0)
      gravity(map, player);
    this_thread::sleep_for(time_between_frames);
    system("cls");
    for(auto & pos : player.prev_positions)
      map[pos] = Map::filler;
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
  Loop();
}