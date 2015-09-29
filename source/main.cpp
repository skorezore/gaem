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
#include "settings.hpp"
#include "curses.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "file.hpp"
#include <experimental/string_view>
#include <functional>
#include <algorithm>
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


void clear_screen() {
	clear();
	refresh();
	move(0, 0);
}

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

bool keyboard_event_loop(gaem_screen & screen, vector<shared_ptr<entity>> & entities) {
	const int key = nonblocking_getch();

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
	return false;
}

void gravity(gaem_screen & screen, vector<shared_ptr<entity>> & entities) {
	for(auto & curent : entities)
		if(curent->position.y < screen.size.y && screen[curent->position.below()] == gaem_screen::filler)
			curent->move_to(curent->position.below());
}

class centralizer : public entity {
	using entity::entity;

	virtual coords movement_destination(const gaem_screen & screen, int) override {
		return {position.x + (position.x < (screen.size.x / 2) ? 1 : -1), position.y};
	}
};

void loop(const function<gaem_screen()> & makescreen) {
	gaem_screen screen = makescreen();
	// Look at that fancy hardcoded screen ^

	unsigned int frames = 0;
	vector<shared_ptr<entity>> entities;
	entities.emplace_back(make_shared<player>('X', A_BOLD));
	entities.emplace_back(make_shared<centralizer>('C'));
	partition(begin(entities), end(entities), [](const auto & ent) {  // Should be done on every added entity
		return !ent->is_player();
	});

	curs_set(0);
	noecho();
	while(true) {
		if(frames++ & 1)
			gravity(screen, entities);
		this_thread::sleep_for(settings().graphics.between_frames);
		move(0, 0);
		for(auto & curent : entities) {
			for(auto & pos : curent->prev_positions)
				screen(pos, gaem_screen::filler);
			curent->prev_positions.clear();
			screen(curent->position, curent->body | curent->colour);
		}
		screen.draw();
		frame_buffer() << string(screen.size.x, '^') << "\n\n"  // Photo-realistic spikes, I know.
		                                                "Use WASD for movement\n"
		                                                "Press Q to quit\n\n"
		                                                "Watch out for the spikes below!\n";
		refresh();

		if(keyboard_event_loop(screen, entities))
			break;

		bool fell = false;  // Need to break outer, so can't put the if in loop
		for(const auto & curent : entities)
			if(curent->position.y > screen.size.y - 2 && curent->is_player()) {
				fell = true;
				break;
			}
		if(fell) {
			curs_set(1);
			frame_buffer() << "\nYou fell to your death. gaem over!\nPress 'r' to restart (10s): ";
			refresh();
			halfdelay(100);
			if(tolower(getch()) == 'r') {
				clear();
				nodelay(stdscr, true);
				loop(makescreen);
			}
			nodelay(stdscr, true);
			break;
		}
	}
	echo();
	curs_set(1);
}

void save_select() {
	auto files = list_files("assets/saevs");
	files.erase(remove_if(begin(files), end(files), [&](const auto & file) { return file.find_first_of(".gaemsaev") == file.size() - 9; }), files.end());
	transform(begin(files), end(files), begin(files), [&](const auto & file) { return file.substr(0, file.size() - 9); });

	frame_buffer() << "Select the level you want to play";

	auto itr = begin(files);
	noecho();
	curs_set(0);
	while(true) {
		bool ctn = true;

		move(2, 0);
		frame_buffer() << '\t';
		copy(begin(files), itr, ostream_iterator<string>(frame_buffer(), "\n\t"));
		attron(A_REVERSE);
		frame_buffer() << *itr << '\n';
		attroff(A_REVERSE);
		frame_buffer() << '\t';
		copy(itr + 1, end(files), ostream_iterator<string>(frame_buffer(), "\n\t"));
		refresh();

		int ch = getch();
		switch(ch) {
			case 'w':
			case 'W':
			case KEY_UP:
				if(itr != begin(files))
					--itr;
				break;

			case 's':
			case 'S':
			case KEY_DOWN:
				if(itr != --end(files))
					++itr;
				break;

			case '\n':
			case '\r':
			case KEY_ENTER:
#ifdef PADENTER
			case PADENTER:
#endif
				ctn = false;
				break;
		}

		if(!ctn)
			break;
	}
	curs_set(1);
	echo();

	loop(bind(load_gaemsaev, "assets/saevs/" + *itr + ".gaemsaev"));
}

void show_credits() {
	static const regex url_regex("[[:space:]]*https?://.*", regex_constants::optimize);
	static const vector<pair<string, attr_t>> lines = []() {
		vector<pair<string, attr_t>> temp;
		ifstream incredits("assets/credits");
		for(string line; getline(incredits, line);)
			temp.emplace_back(line, regex_match(line, url_regex) ? A_UNDERLINE : A_NORMAL);
		return temp;
	}();


	curs_set(0);
	halfdelay(settings().credits.time_between_lines);
	noecho();

	const auto width  = getmaxx(stdscr);
	const auto height = getmaxy(stdscr);
	refresh();

	for(auto start = begin(lines); start != end(lines); ++start) {
		const auto & str = *start;
		const auto cury = getcury(stdscr);
		attron(str.second);
		mvaddstr(cury, (width - str.first.size()) / 2, str.first.c_str());
		attroff(str.second);
		refresh();

		if(cury != height - 1)
			move(cury + 1, 0);
		else
			scrl(1);

		getch();
	}

	echo();
	nodelay(stdscr, true);

	// Because neither nodelay, nor nocbreak work
	while(getch() == ERR) {
	}
	curs_set(1);
}

function<void()> main_menu() {
	static const vector<pair<string_view, function<void()>>> items(
	    {{"Play new Gaem", bind(loop, default_gaemsaev)}, {"Select saev", save_select}, {"Credits", show_credits}, {"Exit", [&]() {}}});

	size_t idx = 1;
	frame_buffer() << "Make your selection:\n\n";
	for(const auto & item : items)
		frame_buffer() << '\t' << idx++ << ". " << item.first << '\n';
	refresh();

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
	scrollok(stdscr, true);
	keypad(stdscr, true);
	if(has_colors())
		start_color();
	main_menu()();
	endwin();
}
