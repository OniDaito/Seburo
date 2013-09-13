/**
* @brief Common Headers that are external
* @file common.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/


///\todo precompile!

#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stddef.h>

#include <stdint.h> 
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

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

#endif
