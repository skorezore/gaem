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


#include "credits.hpp"
#include "../application.hpp"
#include "../settings.hpp"
#include "BearLibTerminal.h"
#include <fstream>
#include <string>


using namespace std;


const regex credits_screen::url_regex("([[:space:]]*)(https?://.*)", regex_constants::optimize);


credits_screen::credits_screen(application & theapp) : screen(theapp), curline(0) {}

int credits_screen::handle_event(int event) {
	static const string lines = []() {
		string temp;
		ifstream incredits("assets/credits");
		for(string line; getline(incredits, line);) {
			temp += regex_replace(line, url_regex, "$1[color=#5050EE]$2[/color]");
			temp += '\n';
		}
		return temp;
	}();


	if(const auto ret = screen::handle_event(event))
		return ret;

	if(curline != string::npos) {
		terminal_clear();
		terminal_print(0, 0, lines.c_str() + curline + !!curline);
		terminal_refresh();

		curline = lines.find('\n', curline + 1);
	} else
		app.end();

	return 0;
}

int credits_screen::halfdelay() const {
	return settings().credits.time_between_lines;
}
