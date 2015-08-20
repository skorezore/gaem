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


#include "settings.hpp"
#include "cpponfig/configuration.hpp"


using namespace std;
using namespace std::chrono;
using namespace cpponfiguration;


const gaem_settings & settings() {
	static configuration cfg("assets/gaem.cfg");
	static gaem_settings setts = [&]() {
		gaem_settings tmp;
		cfg.load();

		cfg.sof_comments = {"This is Gaem's configuration file.", "Modify those values at will, but", "if you break anything, it's your fault."};
		tmp.graphics.between_frames = milliseconds(cfg.get("graphics:time_between_frames", property("75", "In milliseconds")).integer());

		return tmp;
	}();
	static struct save_on_exit_t {
		~save_on_exit_t() {
			cfg.save();
		}
	} save_on_exit;

	return setts;
}
