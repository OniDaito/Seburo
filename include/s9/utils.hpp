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

namespace s9 {


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


	/**
	 * String tokenize with STL
	 * http://www.cplusplus.com/faq/sequences/strings/split/
	 * \todo test split
	 */

	static inline std::vector<std::string> split_string_chars(const std::string& input, const std::string& delimiters) {
		size_t current;
		size_t next = -1;
		std::vector<std::string> tokens;
		do {
			current = next + 1;
			next = input.find_first_of( delimiters, current );
			tokens.push_back(input.substr( current, next - current ));
		}
		while (next != std::string::npos);
	 	return tokens;
	}

	static inline std::vector<std::string> split_string_string(const std::string& input, const std::string& delimiter) {
		size_t current;
		size_t next = -1;
		std::vector<std::string> tokens;
		do {
			current = next + 1;
			next = input.find( delimiter, current );
			tokens.push_back(input.substr( current, next - current ));
		}
		while (next != std::string::npos);
	 	return tokens;
	}


	static inline bool string_contains (const std::string& input, const std::string& contains){
		size_t found = input.find(contains);
  	return found != std::string::npos;
	}


	/**
	 * Remove a char from a string - returns a copy
	 */

	static inline std::string remove_char (const std::string s, const char c) {
		std::string str (s);
		str.erase (std::remove(str.begin(), str.end(), c), str.end());
		return str;
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

		else std::cerr << "SEBURO - Unable to open text file " << filename << std::endl;

		return rval;
	}

	/*
	 * Print Binary Data
	 */

	inline char *itob(int x) {
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

	/*
	 * String Trimming functions
	 */
	 
	// trim from start
	static inline std::string &ltrim(std::string &s) {
	        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	        return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
	        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	        return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
	        return ltrim(rtrim(s));
	}


	/**
	 * Checking at Compile Time
	 * Modern C++ Design: Applied Generic and Design Patterns (C++ in Depth)
	 */

	template <bool> struct CompileTimeChecker {
	 CompileTimeChecker(...) {};
	};

	template <> struct CompileTimeChecker<false> {};

	#define STATIC_CHECK(expr, msg) {\
		class ERROR_##msg {};\
		(void)sizeof(CompileTimeChecker < (expr) != 0 > ((ERROR_##msg)));\
	}

}


#endif
