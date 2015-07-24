// The MIT License (MIT)

// Copyright (c) 2015 Skorezore

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.


#include "frame_buffer.hpp"
#include "game_screen.hpp"
#include "curses.hpp"
#include "entity.hpp"
#include <experimental/string_view>
#include <functional>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>


using namespace std;
using namespace std::chrono;
using namespace std::experimental;


void draw_frame() {
	addstr(frame_buffer->str().c_str());
	refresh();
}

void reset_buffer() {
	move(0, 0);
	frame_buffer->str("");
}

void clear_screen() {
	clear();
	refresh();
	reset_buffer();
}

bool handle_player_movements(game_screen & screen, entity & player) {
	if(kbhit()) {
		char key      = tolower(getch());
		coords newpos = player.position;

		if(key == 'w' && player.position.x > 2 && screen[player.position.below()] != game_screen::filler &&
		   screen[player.position.above()] == game_screen::filler && screen[player.position.above(2)] == game_screen::filler)
			newpos.y -= 2;

		if(key == 'a' && player.position.x > 0 && screen[player.position.left()] == game_screen::filler) {
			--newpos.x;
			if(screen[player.position.below()] == game_screen::filler && screen[player.position.left(2)] == game_screen::filler)
				--newpos.x;
		}

		if(key == 's' && player.position.y < screen.size.y)
			++newpos.y;

		if(key == 'd' && player.position.x < screen.size.x && screen[player.position.right()] == game_screen::filler) {
			++newpos.x;
			if(screen[player.position.below()] == game_screen::filler && screen[player.position.right(2)] == game_screen::filler)
				++newpos.x;
		}

		player.move_to(newpos);

		if(key == 'q')  // Sneaking in that close key
			return true;
	}
	return false;
}

void gravity(game_screen & screen, entity & entity) {
	if(entity.position.y < screen.size.y && screen[entity.position.below()] == game_screen::filler)
		entity.move_to(entity.position.below());
}

void loop() {
	static const auto time_between_frames = 75ms;

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
	entity player('X');
	bool do_gravity = false;

	curs_set(0);
	noecho();
	while(true) {
		if((do_gravity ^= true) % 2 == 0)
			gravity(screen, player);
		this_thread::sleep_for(time_between_frames);
		reset_buffer();
		for(auto & pos : player.prev_positions)
			screen[pos] = game_screen::filler;
		player.prev_positions.clear();
		screen[player.position] = player.body;
		screen.draw();
		*frame_buffer << "^^^^^^^^^^^^^^^^^^^\n\n"  // Photo-realistic spikes, I know.
		                 "Use WASD for movement\n"
		                 "Press Q to quit\n\n"
		                 "X: "
		              << player.position.x << " Y: " << (-player.position.y + screen.size.y) << "\nWatch out for the spikes below!\n";
		draw_frame();

		if(handle_player_movements(screen, player))
			break;

		if(player.position.y > 14) {
			curs_set(1);
			frame_buffer->str("");
			*frame_buffer << "You fell to your death. Game over!\nPress 'r' to restart: ";
			draw_frame();
			if(tolower(getch()) == 'r') {
				clear();
				loop();
			}
			break;
		}
	}
	echo();
	curs_set(1);
}

function<void()> main_menu() {
	static const vector<pair<string_view, function<void()>>> items({{"Play Gaem", loop}, {"Exit", [&]() {}}});

	size_t idx = 1;
	for(const auto & item : items)
		*frame_buffer << idx++ << ". " << item.first << '\n';
	*frame_buffer << "\nPress the key corresponding to your selection: ";
	draw_frame();

	while(true) {
		int key = getch();
		size_t idx;
		stringstream(string(1, static_cast<char>(key))) >> idx;

		if(isdigit(key) && idx > 0 && items.size() >= idx) {
			clear_screen();
			return items[idx - 1].second;
		}
	}
}

int main() {
	initscr();
	main_menu()();
	endwin();
}
