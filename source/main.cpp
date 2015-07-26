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
#include "gaem_screen.hpp"
#include "curses.hpp"
#include "entity.hpp"
#include "player.hpp"
#include <experimental/string_view>
#include <functional>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <cassert>
#include <cmath>


using namespace std;
using namespace std::chrono;
using namespace std::experimental;


void draw_frame() {
	addstr(frame_buffer().str().c_str());
	refresh();
}

void reset_buffer() {
	move(0, 0);
	frame_buffer().str("");
}

void clear_screen() {
	clear();
	refresh();
	reset_buffer();
}

namespace {
	/* Always returns a vector with at least 1 element (the starting position) */
	vector<coords> get_path(const coords & from, const coords & to) {
		static const auto step = [](int v) -> int { return std::copysign(v / v, v); };

		if(from == to)
			return {{to}};

		const bool is_horizontal = from.y == to.y;
		const bool is_vertical   = from.x == to.x;

		assert(is_horizontal || is_vertical);

		const coords delta = is_vertical ? coords{0, step(to.y - from.y)} : coords{step(to.x - from.x), 0};

		vector<coords> path({from});
		while(path.back() != to)
			path.push_back(path.back() + delta);

		return path;
	}
}

bool keyboard_event_loop(game_screen & screen, vector<shared_ptr<entity>> & entities) {
	if(kbhit()) {
		const int key = getch();

		if(tolower(key) == 'q')  // Sneaking in that close key
			return true;

		for(auto & curent : entities) {
			coords destination = curent->movement_destination(screen, key);

			// validate new position
			// validate path is 'free'
			auto path = get_path(curent->position, destination);

			for(auto step = next(begin(path)); step != end(path); ++step) {
				if(screen.is_valid(*step) && screen.is_free(*step))
					curent->move_to(*step);
				else
					break;
			}
		}
	}
	return false;
}

void gravity(game_screen & screen, vector<shared_ptr<entity>> & entities) {
	for(auto & curent : entities)
		if(curent->position.y < screen.size.y && screen[curent->position.below()] == game_screen::filler)
			curent->move_to(curent->position.below());
}

void loop() {
	static const auto time_between_frames = 75ms;

	game_screen screen({16, 16});
	screen({4, 13}, '=');
	screen({5, 13}, '=');
	screen({6, 13}, '=');
	screen({7, 13}, '=');
	screen({8, 13}, '=');
	screen({10, 14}, '=');
	screen({11, 14}, '=');
	// Look at that fancy hardcoded screen ^
	vector<shared_ptr<entity>> entities;
	entities.emplace_back(make_shared<player>('X'));
	bool do_gravity = false;

	curs_set(0);
	noecho();
	while(true) {
		if((do_gravity ^= true) % 2 == 0)
			gravity(screen, entities);
		this_thread::sleep_for(time_between_frames);
		reset_buffer();
		for(auto & curent : entities) {
			for(auto & pos : curent->prev_positions)
				screen(pos, game_screen::filler);
			curent->prev_positions.clear();
			screen(curent->position, curent->body);
		}
		screen.draw();
		frame_buffer() << "^^^^^^^^^^^^^^^^^^^\n\n"  // Photo-realistic spikes, I know.
		                  "Use WASD for movement\n"
		                  "Press Q to quit\n\n"
		                  "Watch out for the spikes below!\n";
		draw_frame();

		if(keyboard_event_loop(screen, entities))
			break;

		bool fell = false;  // Need to break outer, so can't put ihe if in loop
		for(const auto & curent : entities)
			if(curent->position.y > screen.size.y - 2) {
				fell = true;
				break;
			}
		if(fell) {
			curs_set(1);
			frame_buffer().str("");
			frame_buffer() << "\nYou fell to your death. Game over!\nPress 'r' to restart (10s): ";
			draw_frame();
			halfdelay(100);
			if(tolower(getch()) == 'r') {
				clear();
				nodelay(stdscr, true);
				loop();
			}
			nodelay(stdscr, true);
			break;
		}
	}
	echo();
	curs_set(1);
}

function<void()> main_menu() {
	static const vector<pair<string_view, function<void()>>> items({{"Play Gaem", loop}, {"Exit", [&]() {}}});

	size_t idx = 1;
	frame_buffer() << "Make your selection:\n\n";
	for(const auto & item : items)
		frame_buffer() << '\t' << idx++ << ". " << item.first << '\n';
	draw_frame();

	curs_set(0);
	noecho();
	while(true) {
		int key = getch();
		size_t idx;
		stringstream(string(1, static_cast<char>(key))) >> idx;

		if(isdigit(key) && idx > 0 && items.size() >= idx) {
			echo();
			curs_set(1);
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

// Hundreth commit hehe
