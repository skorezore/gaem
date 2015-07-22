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
	if(share)
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
