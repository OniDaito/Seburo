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

/*
 * Basic text file reading
 */

std::string inline textFileRead(std::string filename) {
	std::string line;
	std::string rval;
	std::ifstream myfile (filename.c_str());
	if (myfile.is_open()){
		while ( myfile.good() )	{
			getline (myfile,line);
			rval += line +"\n";
		}
		myfile.close();
	}

	else std::cerr << "S9Gear - Unable to open shader file " << filename << std::endl;

	return rval;
}

/*
 * Print Binary Data
 */

inline char *itob(int x)
{
	static char buff[sizeof(int) * CHAR_BIT + 1];
	int i;
	int j = sizeof(int) * CHAR_BIT - 1;

	buff[j] = 0;
	for(i=0;i<sizeof(int) * CHAR_BIT; i++)
	{
		if(x & (1 << i))
		buff[j] = '1';
	else
		buff[j] = '0';
		j--;
	}
	return buff;
}



#endif
