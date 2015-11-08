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


#include "player.hpp"


using namespace std;


coords player::movement_destination(const gaem_map & screen, int key) {
	switch(key) {
		case TK_W:
		case TK_UP:
				return {position.x, position.y - (terminal_check(TK_SHIFT) ? 1 : (2 * !screen.is_free(position.below())))};

		case TK_S:
		case TK_DOWN:
			return {position.x, position.y + 1};

		case TK_A:
		case TK_LEFT:
			return {position.x - 1 - (terminal_check(TK_SHIFT) ? 0 : screen.is_free(position.below())), position.y};

		case TK_D:
		case TK_RIGHT:
			return {position.x + 1 + (terminal_check(TK_SHIFT) ? 0 : screen.is_free(position.below())), position.y};
	}
	return position;
}

bool player::is_player() const {
	return true;
}
