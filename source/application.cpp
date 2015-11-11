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
#include "screen/main_menu.hpp"
#include "BearLibTerminal.h"


using namespace std;


application::application() : scheduled_screen(make_unique<main_menu_screen>(*this)), cont(true) {}
#include <iostream>
int application::loop() {
cerr << "01\n";
	int retval = 0;
cerr << "02\n";
	while(cont && !retval) {
cerr << "03\n";
		if(scheduled_screen) {
cerr << "04\n";
			terminal_clear();
cerr << "05\n";
			current_screen = move(scheduled_screen);
cerr << "06\n";
			current_screen->handle_event(0);
cerr << "07\n";
		}
cerr << "08\n";
		retval = current_screen->handle_event(terminal_read());
cerr << "09\n";
	}
cerr << "091\n";
	return retval;
}

void application::end() {
	cont = false;
}
