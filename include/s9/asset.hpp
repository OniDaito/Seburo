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
#include "geometry.hpp"
#include "primitive.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace s9 {

	/*
	 * A class that combines a primitive with any geometry set - basic 3D building block outside of a shape
	 */

	template<class T>
	class SEBUROAPI Asset : public Primitive {
	protected:
		struct SharedObj{
			T _geom;
		};
		std::shared_ptr<SharedObj> _obj;

	public:
		Asset() {};
		virtual operator int() const { return _obj.use_count() > 0; };
		Asset(T geom) {_obj.reset(new SharedObj()); _obj->_geom = geom; }
		T getGeometry() { return _obj->_geom; };

	};

	// Handy typedefs

	typedef std::shared_ptr<Asset<GeometryPNF> > AssetPtr;
	typedef Asset<GeometryPNF> AssetBasic;
	typedef Asset<GeometryFullFloat> AssetFull;


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