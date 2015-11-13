// The MIT License (MIT)

// Copyright (c) 2015 Skorezore

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#include "play_gaem.hpp"
#include "../util/pathfinding.hpp"
#include "../application.hpp"
#include "../settings.hpp"
#include "../termutil.hpp"
#include "../player.hpp"
#include "BearLibTerminal.h"
#include <algorithm>
#include <iostream>


using namespace std;
using namespace std::chrono;


class restart_screen : public screen {
private:
	function<gaem_map()> make_map;

public:
	template <class F>
	restart_screen(application & theapp, F && func) : screen(theapp), make_map(std::forward<F>(func)) {}

	virtual int handle_event(int event) override {
		terminal_print(0, 0, "You fell to your death. gaem over!\n"
		                     "Press 'r' to restart (10s):");
		terminal_refresh();
		if(event) {
			if(halfdelay_read(10000) == TK_R)
				app.schedule_screen<play_gaem_screen>(make_map);
			else
				app.end();
		}

		return 0;
	}
};

class centralizer : public entity {
	using entity::entity;

	virtual coords movement_destination(const gaem_map & map, int) override {
		return {position.x + (position.x < (map.size.x / 2) ? 1 : -1), position.y};
	}
};


void play_gaem_screen::setup() {
	entities.emplace_back(make_shared<player>('X', 0xFFFAFAFA));
	entities.emplace_back(make_shared<centralizer>('C'));
	partition(begin(entities), end(entities), [](const auto & ent) {  // Should be done on every added entity
		return !ent->is_player();
	});
}

void play_gaem_screen::gravity() {
	for(auto & curent : entities)
		if(curent->position.y < map.size.y && map[curent->position.below()] == gaem_map::filler)
			curent->move_to(curent->position.below());
}

void play_gaem_screen::keyboard_event_loop(int key) {
	if(key == TK_Q) {  // Sneaking in that close key
		app.end();
		return;
	}

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
}

int play_gaem_screen::handle_event(int event) {
	if(const auto ret = screen::handle_event(event))
		return ret;


	if(!have_help) {
		terminal_print(0, map.size.y, string(map.size.x, '^').c_str());  // Photo-realistic spikes, I know.
		terminal_print(0, map.size.y + 1, "Use WSAD/^v<> for movement\n"
		                                  "Press Q to quit\n"
		                                  "\n"
		                                  "Watch out for the spikes below!");

		terminal_refresh();

		have_help  = true;
		last_frame = high_resolution_clock::now();
	}

	const bool newframe = (high_resolution_clock::now() - last_frame) >= settings().graphics.between_frames;
	if(newframe)
		gravity();

	keyboard_event_loop(event);

	if(newframe) {
		for(auto & curent : entities) {
			for(auto & pos : curent->prev_positions)
				map(pos, gaem_map::filler);  // TODO: don't use/redesign gaem_map
			curent->prev_positions.clear();
			map(curent->position, curent->body);
		}
		map.draw();

		terminal_refresh();

		last_frame = high_resolution_clock::now();
	}

	for(const auto & curent : entities)
		if(curent->position.y > map.size.y - 2 && curent->is_player())
			app.schedule_screen<restart_screen>(make_map);

	return 0;
}

int play_gaem_screen::halfdelay() const {
	return 1;
}
