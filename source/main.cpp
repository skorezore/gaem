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


#include "gaem_map.hpp"
#include "termutil.hpp"
#include "settings.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "file.hpp"
#include "BearLibTerminal.h"
#include <experimental/string_view>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <cassert>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <cmath>
#include <regex>


using namespace std;
using namespace std::chrono;
using namespace std::experimental;


namespace {
	/* Always returns a vector with at least 1 element (the starting position) */
	vector<coords> get_path(const coords & from, const coords & to) {
		static const auto step = [](int v) -> int { return copysign(v / v, v); };

		if(from == to)
			return {{from}};

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

bool keyboard_event_loop(gaem_map & map, vector<shared_ptr<entity>> & entities) {
	const int key = nonblocking_read();

	if(tolower(key) == 'q')  // Sneaking in that close key
		return true;

	for(auto & curent : entities) {
		coords destination = curent->movement_destination(map, key);

		// validate new position
		// validate path is 'free'
		auto path = get_path(curent->position, destination);

		for(auto step = next(begin(path)); step != end(path); ++step) {
			if(map.is_valid(*step) && map.is_free(*step))
				curent->move_to(*step);
			else
				break;
		}
	}
	return false;
}

void gravity(gaem_map & map, vector<shared_ptr<entity>> & entities) {
	for(auto & curent : entities)
		if(curent->position.y < map.size.y && map[curent->position.below()] == gaem_map::filler)
			curent->move_to(curent->position.below());
}

class centralizer : public entity {
	using entity::entity;

	virtual coords movement_destination(const gaem_map & map, int) override {
		return {position.x + (position.x < (map.size.x / 2) ? 1 : -1), position.y};
	}
};

void loop(const function<gaem_map()> & makemap) {
	gaem_map map = makemap();
	// Look at that fancy hardcoded screen ^

	unsigned int frames = 0;
	vector<shared_ptr<entity>> entities;
	entities.emplace_back(make_shared<player>('X', 0xFFFAFAFA));
	entities.emplace_back(make_shared<centralizer>('C'));
	partition(begin(entities), end(entities), [](const auto & ent) {  // Should be done on every added entity
		return !ent->is_player();
	});

	terminal_set("input.cursor-symbol = 0x20;");
	// noecho();
	while(true) {
		if(frames++ & 1)
			gravity(map, entities);
		this_thread::sleep_for(settings().graphics.between_frames);
		for(auto & curent : entities) {
			for(auto & pos : curent->prev_positions)
				map(pos, gaem_map::filler);
			curent->prev_positions.clear();
			map(curent->position, curent->body | curent->colour);
		}
		map.draw();
		terminal_print(0, map.size.y, string(map.size.x, '^').c_str());  // Photo-realistic spikes, I know.
		terminal_print(0, map.size.y + 1, "\n\n"
		                                  "Use WSAD/^v<> for movement\n"
		                                  "Press Q to quit\n\n"
		                                  "Watch out for the spikes below!\n");
		terminal_refresh();

		if(keyboard_event_loop(map, entities))
			break;

		bool fell = false;  // Need to break outer, so can't put the if in loop
		for(const auto & curent : entities)
			if(curent->position.y > map.size.y - 2 && curent->is_player()) {
				fell = true;
				break;
			}
		if(fell) {
			terminal_set("input.cursor-symbol = 0x4F;");
			terminal_print(0, map.size.y + 2, "\n\n"
			                                  "\nYou fell to your death. gaem over!\nPress 'r' "
			                                  "to restart (10s): ");
			terminal_refresh();
			// halfdelay(100);
			// TODO: implement halfdelay
			terminal_delay(10000);
			if(terminal_has_input() && terminal_read() == 'r') {
				terminal_clear();
				// nodelay(stdscr, true);
				loop(makemap);
			}
			// nodelay(stdscr, true);
			break;
		}
	}
	// echo();
	terminal_set("input.cursor-symbol = 0x4F;");
}

void save_select() {
	auto files = list_files("assets/saevs");
	files.erase(remove_if(begin(files), end(files), [&](const auto & file) { return file.find_first_of(".gaemsaev") == file.size() - 9; }), files.end());
	transform(begin(files), end(files), begin(files), [&](const auto & file) { return file.substr(0, file.size() - 9); });

	terminal_print(0, 0, "Select the level you want to play");

	auto itr = begin(files);
	// noecho();
	terminal_set("input.cursor-symbol = 0x20;");
	for(bool ctn = true; ctn;) {
		int y = 2;

		for_each(begin(files), itr, [&](const auto & saev) { terminal_printf(4, y++, "[bkcolor=black][color=white]%s", saev.c_str()); });
		terminal_printf(4, y++, "[bkcolor=white][color=black]%s", itr->c_str());
		for_each(itr + 1, end(files), [&](const auto & saev) { terminal_printf(4, y++, "[bkcolor=black][color=white]%s", saev.c_str()); });
		terminal_refresh();

		switch(terminal_read()) {
			case TK_W:
			case TK_UP:
				if(itr != begin(files))
					--itr;
				break;

			case TK_S:
			case TK_DOWN:
				if(itr != --end(files))
					++itr;
				break;

			case TK_ENTER:
			case TK_KP_ENTER:
				ctn = false;
				break;
		}
	}
	terminal_set("input.cursor-symbol = 0x4F;");
	// echo();

	loop(bind(load_gaemsaev, "assets/saevs/" + *itr + ".gaemsaev"));
}

void show_credits() {
	static const regex url_regex("[[:space:]]*https?://.*", regex_constants::optimize);
	static const vector<string> lines = []() {
		vector<string> temp;
		ifstream incredits("assets/credits");
		for(string line; getline(incredits, line);)
			temp.emplace_back((regex_match(line, url_regex) ? "[color=blue]" : +"") + line);
		return temp;
	}();


	terminal_set("input.cursor-symbol = 0x20;");
	// halfdelay(settings().credits.time_between_lines);
	// noecho();

	const auto height = terminal_state(TK_HEIGHT);
	terminal_refresh();

	int y = 0;
	for(auto itr = begin(lines); itr != end(lines); ++itr) {
		const auto & str = *itr;
		terminal_print(0, y, str.c_str());
		terminal_refresh();

		if(y < height)
			++y;
		else {
			terminal_clear();
			for(auto itr = begin(lines) + (y - height); itr != end(lines); ++itr)
				terminal_print(0, y, str.c_str());
			terminal_refresh();
		}

		// getch();
		terminal_delay(settings().credits.time_between_lines);
	}

	terminal_read();
}

function<void()> main_menu() {
	static const vector<pair<string_view, function<void()>>> items(
	    {{"Play new Gaem", bind(loop, default_gaemsaev)}, {"Select saev", save_select}, {"Credits", show_credits}, {"Exit", [&]() {}}});

	size_t idx = 1;
	terminal_print(0, 0, "Make your selection:");
	for(const auto & item : items) {
		terminal_print(4, idx + 2, (to_string(idx) + ". " + item.first.data()).c_str());
		++idx;
	}
	terminal_refresh();

	terminal_set("input.cursor-symbol = 0x20;");
	// noecho();
	while(true) {
		int key = terminal_read();
		size_t idx = key - TK_1;

		cerr << hex << key << '\n';
		if(key >= TK_1 && key <= TK_9 && items.size() >= idx) {
			// echo();
			terminal_set("input.cursor-symbol = 0x4F;");
			terminal_clear();
			return items[idx].second;
		}
	}
}

int main() {
	cerr << "0\n";
	if(!terminal_open()) {
		cerr << "Couldn't initialize BearLibTerminal!\nSee \"bearlibterminal.log\" for details.\n";
		return 1;
	}
	cerr << "1\n";
	terminal_set("window.title='Gaem'; input.mouse-cursor = false; input.filter='keyboard';");
	cerr << "2\n";

	try {
		cerr << "3\n";
		main_menu()();
	} catch(const exception & exc) {
		cerr << "Exception captured: \"" << exc.what() << "\"\n";
		return 2;
	} catch(...) {
		cerr << "Unknown exception captured.\n";
		return 2;
	}

	terminal_close();
}
