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
	 * \TODO potentially either go TinyXML2 or compile in the STL for this class
	 * \TODO Better integration with the file system (indeed we dont always need a file)
	 * Example Usage
	 */


	/**
	 * Basic iterator for multiple values
	 */

	class SEBUROAPI XMLIterator {
	public:
		XMLIterator() {pElement = nullptr; };
			
		virtual operator int() const { return pElement != nullptr; };
		void Set(TiXmlElement* p) {pElement = p; };
		
		// Assuming the first child is an actual text value in the XML :S
		template <typename T>
		void SetValue(T val) { if (pElement != nullptr) { std::string s = ToStringS9(val); pElement->FirstChild()->SetValue( s.c_str() ); std::cout << s << std::endl; } }

		std::string Value();	

		std::string operator*();

		bool Next();
	protected:
		TiXmlElement* pElement;
	};


	template<>
	void XMLIterator::SetValue(std::string val) {pElement->SetValue(val.c_str()) ; };

	/**
	 * Basic class that wraps tinyxml providing a dictionary like path
	 * to values inside a tinyxml document
	 */

	class SEBUROAPI XMLSettings {
	public:
		bool LoadFile(std::string filename);
		bool LoadFile(s9::File file);
		XMLIterator operator[](std::string s);
		XMLIterator Iterator(std::string s);

		bool SaveFile() { return SaveFile(obj_->path);}
		bool SaveFile(s9::File file) {
			return obj_->doc.SaveFile( file.final_path().c_str() );
		}

		bool SaveFile(std::string path) {
			return obj_->doc.SaveFile( path.c_str() );
		}
	
	protected:
		
		class SharedObject{
		public:
			std::string path;
			TiXmlDocument doc;
		};
		
		std::shared_ptr<SharedObject> obj_;
	};
}




#endif
