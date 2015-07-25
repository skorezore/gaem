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


#include "game_screen.hpp"
#include "frame_buffer.hpp"


using namespace std;


const char game_screen::filler = ' ';


game_screen::game_screen(const coords & sz) : size(sz) {
	coords xy{};
	for(; xy.x < size.x; ++xy.x) {
		for(; xy.y < size.y; ++xy.y)
			map.emplace(xy, filler);
		xy.y = 0;
	}
}

void game_screen::draw() {
	for(auto itr = map.begin(); itr != map.end(); ++itr) {
		for(int i = 1; i < size.x; ++i)
			frame_buffer() << itr++->second;
		frame_buffer() << '\n';
	}
}

void game_screen::reset() {
	for(auto & pr : map)
		pr.second = filler;
}

reference_proxy<char> game_screen::operator[](const coords & xy) {
    auto match = map.find(xy);
    if (map.end() == match)
        return filler;
    else
        return match->second;
}

bool game_screen::is_valid(coords pos) const {
    return map.end() != map.find(pos);
}
