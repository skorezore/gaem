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


#include <fstream>
#include <iterator>
#include "gaem_screen.hpp"
#include "frame_buffer.hpp"
#include "coords.hpp"


using namespace std;


const chtype gaem_screen::filler = ' ';


gaem_screen::gaem_screen(const coords & sz) : size(sz), map(size.y, decltype(map)::value_type(size.x, filler)) {}

void gaem_screen::draw() {
	for(const auto & row : map) {
		copy(begin(row), end(row), ostream_iterator<chtype_indicator>(frame_buffer(), ""));
		frame_buffer() << '\n';
	}
}

void gaem_screen::reset() {
	for(auto & row : map)
		fill(row.begin(), row.end(), filler);
}

bool gaem_screen::is_valid(const coords & xy) const {
	return xy.x < size.x && xy.y < size.y;
}

bool gaem_screen::is_free(const coords & xy) const {
	return is_valid(xy) && map[xy.y][xy.x] == filler;
}

chtype gaem_screen::operator[](const coords & xy) {
	if(is_valid(xy))
		return map[xy.y][xy.x];
	else
		return gaem_screen::filler;
}

void gaem_screen::operator()(const coords & xy, chtype newval) {
	if(is_valid(xy))
		map[xy.y][xy.x] = newval;
}


gaem_screen load_gaemsaev(const string & path) {
	ifstream file(path);

	coords temp;
	char content;
	int x, y;
	file >> x >> content >> y;

	gaem_screen screen({x, y});
	while(file >> temp >> content)
		screen(temp, content);

	return screen;
}
