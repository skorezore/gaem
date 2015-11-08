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
#include "gaem_map.hpp"
#include "coords.hpp"


using namespace std;


const char gaem_map::filler = ' ';


gaem_map::gaem_map(const coords & sz) : size(sz), map(size.y, decltype(map)::value_type(size.x, filler)) {}

void gaem_map::draw() {
	int y = 0;
	for(const auto & row : map)
		terminal_print(0, y++, row.c_str());
}

void gaem_map::reset() {
	for(auto & row : map)
		fill(row.begin(), row.end(), filler);
}

bool gaem_map::is_valid(const coords & xy) const {
	return xy.x < size.x && xy.y < size.y;
}

bool gaem_map::is_free(const coords & xy) const {
	return is_valid(xy) && map[xy.y][xy.x] == filler;
}

char gaem_map::operator[](const coords & xy) {
	if(is_valid(xy))
		return map[xy.y][xy.x];
	else
		return gaem_map::filler;
}

void gaem_map::operator()(const coords & xy, char newval) {
	if(is_valid(xy))
		map[xy.y][xy.x] = newval;
}


gaem_map load_gaemsaev(const string & path) {
	ifstream file(path);

	if(!file.is_open())
		return default_gaemsaev();

	coords temp;
	char content;
	int x, y;
	file >> x >> content >> y;

	gaem_map screen({x, y});
	while(file >> temp >> content)
		screen(temp, content);

	return screen;
}

gaem_map default_gaemsaev() {
	gaem_map screen({16, 16});
	screen({4, 13}, '=');
	screen({5, 13}, '=');
	screen({6, 13}, '=');
	screen({7, 13}, '=');
	screen({8, 13}, '=');
	screen({10, 14}, '=');
	screen({11, 14}, '=');
	return screen;
}
