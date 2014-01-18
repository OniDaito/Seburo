/**
* @brief Maths Utils
* @file math_utils.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/1/2013
*
*/

#include "common.hpp"

/// Useful maths stuff

#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP


static const double PI = M_PI;

inline static double RadToDeg(const double &x) { return x * 180.0 / PI; }
inline static double DegToRad(const double &x) { return x * PI / 180.0; }

inline static float RadToDeg(const float &x) { return x * 180.0 / PI; }
inline static float DegToRad(const float &x) { return x * PI / 180.0; }


#endif
