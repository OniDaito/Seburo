/**
* @brief Utilities header
* @file utils.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 26/04/2012
*
*/


#ifndef UTILS_HPP
#define UTILS_HPP

#include "common.hpp"

template<class T> inline std::string toStringS9(const T& t) {
	std::ostringstream stream;
	stream << t;
	return stream.str();
}


template<class T> inline T fromStringS9(const std::string& s) {
	std::istringstream stream (s);
	T t;
	stream >> t;
	return t;
}


#endif
