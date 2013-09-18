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


namespace s9{

	/*
	 * Basic iterator for multiple values
	 */
	class SEBUROAPI XMLIterator {
	public:
		XMLIterator() {pElement = NULL; };
		std::string operator[](const char *s); // There is an implicit built in!
		virtual operator int() const { return pElement != NULL; };
		void set(TiXmlElement* p) {pElement = p; };
		std::string operator*();
		bool next();
	protected:
		TiXmlElement* pElement;
	};

	/*
	 * Basic class that wraps tinyxml providing a dictionary like path
	 * to values inside a tinyxml document
	 */

	class SEBUROAPI XMLSettings {
	public:
		bool loadFile(std::string filename);
		std::string operator[](std::string s);
		XMLIterator iterator(std::string s);
	
	protected:
		
		
		class SharedObj{
		public:
			std::string mFilename;
			boost::shared_ptr<TiXmlDocument> mDoc;
			std::map < std::string, std::string > mCache;
		};
		
		boost::shared_ptr<SharedObj> mObj;
	};
}




#endif
