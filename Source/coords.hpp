#pragma once
#ifndef COORDS_HPP
#define COORDS_HPP


#include <iosfwd>


struct coords {
  int x;
  int y;

  coords below(int factor = 1) const;
  coords above(int factor = 1) const;
};

std::ostream & operator<<(std::ostream & strm, const coords & xy);
bool operator<(const coords & lhs, const coords & rhs);


#endif  // COORDS_HPP
