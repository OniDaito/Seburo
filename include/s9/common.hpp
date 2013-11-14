/**
* @brief Common Headers that are external
* @file common.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/


///\todo precompile! :P

#ifndef S9_COMMON_HPP
#define S9_COMMON_HPP

// STL C++
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <fstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


// Basic C like headers

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <stddef.h>
#include <stdint.h> 


#ifdef _SEBURO_WIN32
#include "win32/s9win32.hpp"
#else
#include <unistd.h>
#endif


#if defined(_SEBURO_WIN32) && defined(_SEBURO_BUILD_DLL)
  #define SEBUROAPI __declspec(dllexport)
#else
  #define SEBUROAPI
#endif


///\todo check if GLM relies on OpenGL at some point

#define GLM_SWIZZLE 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/string_cast.hpp>

#if defined(_SEBURO_WIN32) && defined(_SEBURO_BUILD_DLL)
  #define SEBUROAPI __declspec(dllexport)
#else
  #define SEBUROAPI
#endif


/// Type definitions
typedef uint8_t byte_t;


#endif
