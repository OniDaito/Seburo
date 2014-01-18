/**
* @brief S9 XML Loading
* @file s9xml.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 11/07/2012
*
*/


#ifndef S9_XML_HPP
#define S9_XML_HPP

#include <numeric> 
#include <map>

#include "tinyxml.h"
#include "common.hpp"
#include "string_utils.hpp"
#include "file.hpp"


namespace s9{

	/**
	 * The S9 XML Classes. Essentially creates an iterator and a search function for xml files
	 * Example Usage
	 */


   /* uint32_t w = fromStringS9<uint32_t> ( _obj->_settings["TouchwallApp/cameras/width"]);
		uint32_t h = fromStringS9<uint32_t> ( _obj->_settings["TouchwallApp/cameras/height"]);
		uint32_t f = fromStringS9<uint32_t> ( _obj->_settings["TouchwallApp/cameras/fps"]);
		XMLIterator i = _obj->_settings.iterator("TouchwallApp/cameras/cam");
		while (i){

			string dev = i["dev"];
			VidCam p (dev,w,h,f);
			_obj->_cameras.push_back(p);
			CVVidCam c(_obj->_cameras.back());
			c.loadParameters("./data/" + i["in"]);
			_obj->_cv_cameras.push_back(c);

			i.next();
		}
	*/

	/**
	 * Basic iterator for multiple values
	 */

	class SEBUROAPI XMLIterator {
	public:
		XMLIterator() {pElement = NULL; };
		std::string operator[](const char *s); // There is an implicit built in!
		virtual operator int() const { return pElement != NULL; };
		void Set(TiXmlElement* p) {pElement = p; };
		std::string operator*();
		bool Next();
	protected:
		TiXmlElement* pElement;
	};

	/**
	 * Basic class that wraps tinyxml providing a dictionary like path
	 * to values inside a tinyxml document
	 */

	class SEBUROAPI XMLSettings {
	public:
		bool LoadFile(std::string filename);
		bool LoadFile(s9::File file);
		std::string operator[](std::string s);
		XMLIterator Iterator(std::string s);
	
	protected:
		
		
		class SharedObject{
		public:
			std::string path;
			std::shared_ptr<TiXmlDocument> doc;
			std::map < std::string, std::string > cache;
		};
		
		std::shared_ptr<SharedObject> obj_;
	};
}




#endif
