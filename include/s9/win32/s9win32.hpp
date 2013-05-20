/**
* @brief Stuff Windows Needs
* @file common.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 15/05/2013
*
*/


#include <stddef.h>
#include <windows.h>

//#define _HAS_CPP0X 0
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__))
 #define _WIN32
#endif /* _WIN32 */


#define WIN32_LEAN_AND_MEAN

#if defined(_SEBURO_WIN32) && defined(_SEBURO_BUILD_DLL)
 /* We are building a Win32 DLL */
  #define SEBUROAPI __declspec(dllexport)
#elif defined(__GNUC__) && defined(_GLFW_BUILD_DLL)
  #define SEBUROAPI __attribute__((visibility("default")))
#else
  #define SEBUROAPI
#endif


typedef double double_t;