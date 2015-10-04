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


#include <ostream>
#include <istream>


struct coords {
	int x;
	int y;

	constexpr coords below(int delta = 1) const {
		return {x, y + delta};
	}
	constexpr coords above(int delta = 1) const {
		return {x, y - delta};
	}
	constexpr coords left(int delta = 1) const {
		return {x - delta, y};
	}
	constexpr coords right(int delta = 1) const {
		return {x + delta, y};
	}
};


inline std::ostream & operator<<(std::ostream & strm, const coords & xy) {
	strm << "{x=" << xy.x << ",y=" << xy.y << '}';
	return strm;
}

inline std::istream & operator>>(std::istream & strm, coords & xy) {
	char ignore;
	strm >> ignore >> ignore >> ignore >> xy.x >> ignore >> ignore >> ignore >> xy.y >> ignore;
	return strm;
}

inline bool operator<(const coords & lhs, const coords & rhs) {
	return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}

inline bool operator==(const coords & lhs, const coords & rhs) {
	return lhs.y == rhs.y && lhs.x == rhs.x;
}

inline bool operator!=(const coords & lhs, const coords & rhs) {
	return lhs.y != rhs.y || lhs.x != rhs.x;
}

inline coords operator+(coords const & p, coords const & delta) {
	return {p.x + delta.x, p.y + delta.y};
}

inline coords operator-(coords const & p, coords const & delta) {
	return {p.x - delta.x, p.y - delta.y};
}


//          Bulbasaur
//                 __....___ ,  .
//             _.-~ __...--~~ ~/\
//   Amw      / /  /          |  |
//           | |  |            \  \
//     __ _..---..-~\           |  |
//    |  ~  .-~-.    \-.__      /  |
//    /     \.-~        .-~-._/   //
//   |/-. <| __  .-\    \     \_ //
//   || o\   \/ /o  |    ~-.-~  \/
//  /  ~~        ~~              |
//  \__         ___--/   \  _-~-  \
//   / ~~--.--~~    /     |/   __  |
//  |/\ \          |_~|   /    \|  |
//  |\/  \__       /_-   /\        |
//  |_ __| |`~-.__|_ _ _/  \ _ _ _/
//  ' '  ' ' ''   ' ' '     ' ` `
