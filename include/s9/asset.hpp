/*
* @brief GLFW Application
* @file asset.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef S9_ASSET_HPP
#define S9_ASSET_HPP

#include "common.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace s9 {



	/*
 	 * A wrapper around the Assimp library
 	 * \todo do something better with pScene
 	 */

#ifndef _SEBURO_WIN32

		class SEBUROAPI AssetImporter {
		public:
			static AssetBasic load(std::string filename);

			virtual ~AssetImporter();

		protected:

			static AssetPtr _load (const struct aiScene *sc, const struct aiNode* nd, AssetPtr p);
			static const struct aiScene* pScene;

		};

#endif
	

}


#endif