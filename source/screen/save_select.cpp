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


#include "save_select.hpp"
#include "../application.hpp"
#include "../file.hpp"
#include "play_gaem.hpp"
#include "BearLibTerminal.h"
#include <algorithm>


using namespace std;


save_select_screen::save_select_screen(application & theapp) : screen(theapp) {}

int save_select_screen::handle_event(int event) {
	if(const auto ret = screen::handle_event(event))
		return ret;

	if(saves.empty()) {
		saves = list_files("assets/saevs");
		saves.erase(remove_if(begin(saves), end(saves), [&](const auto & file) { return file.find_first_of(".gaemsaev") == file.size() - 9; }), saves.end());
		transform(begin(saves), end(saves), begin(saves), [&](const auto & file) { return file.substr(0, file.size() - 9); });

		current = saves.begin();

		terminal_print(0, 0, "Select the level you want to play");
	}


	int y = 2;

	for_each(begin(saves), current, [&](const auto & saev) { terminal_printf(4, y++, "[bkcolor=black][color=white]%s", saev.c_str()); });
	terminal_printf(4, y++, "[bkcolor=white][color=black]%s", current->c_str());
	for_each(current + 1, end(saves), [&](const auto & saev) { terminal_printf(4, y++, "[bkcolor=black][color=white]%s", saev.c_str()); });
	terminal_refresh();

	switch(event) {
		case TK_W:
		case TK_UP:
			if(current != begin(saves))
				--current;
			break;

		case TK_S:
		case TK_DOWN:
			if(current != --end(saves))
				++current;
			break;

		case TK_ENTER:
		case TK_KP_ENTER:
			app.schedule_screen<play_gaem_screen>(bind(load_gaemsaev, "assets/saevs/" + *current + ".gaemsaev"));
			break;
	}

	return 0;
}
