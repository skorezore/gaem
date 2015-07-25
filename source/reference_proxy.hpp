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
#ifndef REFERENCE_PROXY_HPP
#define REFERENCE_PROXY_HPP


#include <ostream>


template <class T>
class reference_proxy {
	friend std::ostream & operator<<(std::ostream & strm, const reference_proxy & px) {
		strm << '[' << px.mem << ']';
		return strm;
	}

private:
	T & mem;
	bool share;

public:
	reference_proxy(T & ref, bool shr = true) noexcept;
	reference_proxy(const T & ref) noexcept;

	reference_proxy & operator=(const T & val) noexcept(noexcept(mem = val));

	bool operator==(const T & val) noexcept(noexcept(mem == val));
	bool operator!=(const T & val) noexcept(noexcept(mem != val));
};

template <class T>
bool operator<(const reference_proxy<T> & lhs, const reference_proxy<T> & rhs) {
	return lhs.mem < rhs.mem;
}

template <class T>
bool operator>(const reference_proxy<T> & lhs, const reference_proxy<T> & rhs) {
	return lhs.mem > rhs.mem;
}


template <class T>
reference_proxy<T>::reference_proxy(T & ref, bool shr) noexcept : mem(ref), share(shr) {}

template <class T>
reference_proxy<T>::reference_proxy(const T & ref) noexcept : reference_proxy(const_cast<T &>(ref), false) {}

template <class T>
reference_proxy<T> & reference_proxy<T>::operator=(const T & val) noexcept(noexcept(mem = val)) {
	if(share)  // WUT - so we silently swallow assignments... :( BAD ideas
		mem = val;
	return *this;
}

template <class T>
bool reference_proxy<T>::operator==(const T & val) noexcept(noexcept(mem == val)) {
	return mem == val;
}

template <class T>
bool reference_proxy<T>::operator!=(const T & val) noexcept(noexcept(mem != val)) {
	return mem != val;
}


#endif  // REFERENCE_PROXY_HPP
