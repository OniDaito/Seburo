/**
* @brief Loading a model or Asset with Assimp
* @file asset.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 26/07/2012
*
*/

#include "s9/asset.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;


/*
 * Recursive load function. Dependent on the actual type of the geom
 */ 

