/**
* @brief Compiler Utilities that might be handy
* @file compiler_utils.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 14/1/2013
*
*/
  
/**
 * Checking at Compile Time
 * Modern C++ Design: Applied Generic and Design Patterns (C++ in Depth)
 */



#ifndef COMPILER_UTILS_HPP
#define COMPILER_UTILS_HPP

template <bool> struct CompileTimeChecker {
 CompileTimeChecker(...) {};
};

template <> struct CompileTimeChecker<false> {};

#define STATIC_CHECK(expr, msg) {\
  class ERROR_##msg {};\
  (void)sizeof(CompileTimeChecker < (expr) != 0 > ((ERROR_##msg)));\
}

#endif