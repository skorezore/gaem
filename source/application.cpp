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


#include "application.hpp"
#include "termutil.hpp"
#include "BearLibTerminal.h"


using namespace std;


application::application() : keep_going(true) {}

int application::loop() {
	if(!current_screen && !scheduled_screen)
		throw logic_error("No screen to display");

	int retval = 0;
	while(keep_going && !retval) {
		if(scheduled_screen) {
			terminal_clear();
			current_screen = move(scheduled_screen);
			current_screen->handle_event(0);
		}

		const auto halfdelay = current_screen->halfdelay();
		if(halfdelay < 0)
			retval = current_screen->handle_event(terminal_peek());
		else if(halfdelay == 0)
			retval = current_screen->handle_event(terminal_read());
		else
			retval = current_screen->handle_event(halfdelay_read(halfdelay));
	}
	return retval;
}

void application::end() {
	keep_going = false;
}
