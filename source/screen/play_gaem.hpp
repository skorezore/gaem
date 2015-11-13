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


#pragma once


#include "../gaem_map.hpp"
#include "../screen.hpp"
#include "../entity.hpp"
#include <functional>
#include <chrono>
#include <memory>
#include <vector>


class play_gaem_screen : public screen {
private:
	gaem_map map;
	// Look at that fancy hardcoded screen ^
	std::function<gaem_map()> make_map;
	std::vector<std::shared_ptr<entity>> entities;
	bool have_help;
	std::chrono::high_resolution_clock::time_point last_frame;

	void setup();
	void gravity();
	void keyboard_event_loop(int key);

public:
	template <class F>
	play_gaem_screen(application & theapp, F && func) : screen(theapp), map(func()), make_map(std::forward<F>(func)), have_help(false) {
		setup();
	}

	virtual int handle_event(int event) override;
	virtual int halfdelay() const override;
};
