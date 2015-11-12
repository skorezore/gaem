// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

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


#include "pathfinding.hpp"
#include <cassert>
#include <cmath>


using namespace std;


/* Always returns a vector with at least 1 element (the starting position) */
vector<coords> get_path(const coords & from, const coords & to) {
	static const auto step = [](int v) -> int { return copysign(v / v, v); };

	if(from == to)
		return {{from}};

	const bool is_horizontal = from.y == to.y;
	const bool is_vertical   = from.x == to.x;

	assert(is_horizontal || is_vertical);

	const coords delta = is_vertical ? coords{0, step(to.y - from.y)} : coords{step(to.x - from.x), 0};

	vector<coords> path({from});
	while(path.back() != to)
		path.push_back(path.back() + delta);

	return path;
}
