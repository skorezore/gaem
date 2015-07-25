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

struct coords {
	int x;
	int y;

    constexpr coords below(int factor) const { return { x, y + factor }; } 
    constexpr coords above(int factor) const { return { x, y - factor }; } 
    constexpr coords left (int factor) const { return { x - factor, y }; } 
    constexpr coords right(int factor) const { return { x + factor, y }; } 

};

inline std::ostream & operator<<(std::ostream & strm, const coords & xy) {
    return strm << "{x=" << xy.x << ",y=" << xy.y << '}';
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

inline coords operator+(coords const& p, coords const& delta) {
    return { p.x + delta.x, p.y + delta.y };
}

inline coords operator-(coords const& p, coords const& delta) {
    return { p.x - delta.x, p.y - delta.y };
}
