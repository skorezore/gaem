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


#include "main_menu.hpp"
#include "../application.hpp"
#include "credits.hpp"
#include "BearLibTerminal.h"
#include <experimental/string_view>
#include <functional>
#include <vector>


using namespace std;
using namespace std::experimental;


class play_screen : public screen {  // TODO: implement
public:
	play_screen(application & theapp) : screen(theapp) {}
};
class save_select_screen : public screen {  // TODO: implement
public:
	save_select_screen(application & theapp) : screen(theapp) {}
};


main_menu_screen::main_menu_screen(application & theapp) : screen(theapp), drawn(false) {}

int main_menu_screen::handle_event(int event) {
	static const vector<pair<string_view, function<void()>>> items({{"Play new Gaem", [&]() { app.schedule_screen<play_screen>(); }},
	                                                                {"Select saev", [&]() { app.schedule_screen<save_select_screen>(); }},
	                                                                {"Credits", [&]() { app.schedule_screen<credits_screen>(); }},
	                                                                {"Exit", [&]() { app.end(); }}});


	if(const auto ret = screen::handle_event(event))
		return ret;
	if(!drawn) {
		size_t idx = 1;
		terminal_print(0, 0, "Make your selection:");
		for(const auto & item : items) {
			terminal_print(4, idx + 1, (to_string(idx) + ". " + item.first.data()).c_str());
			++idx;
		}
		terminal_refresh();
		drawn = true;
	}

	size_t idx = event - TK_1;

	if(event >= TK_1 && event <= TK_9 && items.size() >= idx) {
		(items[idx].second)();
		return 0;
	}
}
