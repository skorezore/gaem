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


#include "application.hpp"
#include "screen/main_menu.hpp"
#include <BearLibTerminal.h>
#include <stdexcept>
#include <iostream>


using namespace std;


extern "C" int getch(void);


int main() {
	if(!terminal_open()) {
		cerr << "Couldn't initialize BearLibTerminal!\nSee \"bearlibterminal.log\" for details.\n";
		return 1;
	}
	terminal_set("window.title='Gaem'; input: mouse-cursor = false, filter='keyboard';");

	int retval = 0;
	try {
		application app;
		app.schedule_screen<main_menu_screen>();
		app.loop();
	} catch(const exception & exc) {
		cerr << "Exception captured: \"" << exc.what() << "\"\n";
		retval = 2;
	} catch(...) {
		cerr << "Unknown exception captured.\n";
		retval = 2;
	}

	terminal_close();

	if(retval) {
		cerr << "Please report that to the developers on the issue tracker at github.com/skorezore/gaem,\n"
		        "or fire them an e-mail at nabijaczleweli@gmail.com and/or skorezore@gmail.com\n"
		        "\n"
		        "Press any key to continue...";
		getch();
	}

	return retval;
}
