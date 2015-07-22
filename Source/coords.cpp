#include "coords.hpp"
#include <ostream>


using namespace std;


coords coords::below(int factor) const {
	return {x, y + factor};
}

coords coords::above(int factor) const {
	return {x, y - factor};
}

ostream & operator<<(ostream & strm, const coords & xy) {
	strm << "{x=" << xy.x << ",y=" << xy.y << '}';
	return strm;
}

bool operator<(const coords & lhs, const coords & rhs) {
	return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}
