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


#pragma once
#ifndef GAEM_SCREEN_HPP
#define GAEM_SCREEN_HPP


#include "coords.hpp"
#include "curses.hpp"
#include <string>
#include <vector>


class gaem_screen {
public:
	static const chtype filler;

	const coords size;

	gaem_screen(const coords & sz);

	void draw();
	void reset();

	bool is_valid(const coords & pos) const;
	bool is_free(const coords & pos) const;

	chtype operator[](const coords & xy);
	void operator()(const coords & xy, chtype newval);

private:
	std::vector<std::vector<chtype>> map;
};


gaem_screen load_gaemsaev(const std::string & path);


#endif  // gaem_SCREEN_HPP
