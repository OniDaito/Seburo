/**
* @brief S9 XML Reading / Writing Classes
* @file camera.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/xml_parse.hpp"

using namespace std;
using namespace s9; 


TiXmlElement* _find(TiXmlElement *p, std::vector<std::string> s){
	p = p->FirstChildElement(s[0].c_str());
	s.erase(s.begin());
	if (s.size() > 0 && p) {
		return _find(p,s);
	}
	return p;
}


TiXmlElement* Find(std::vector<std::string> strs, TiXmlElement *pRoot){
	
	string s = accumulate( strs.begin(), strs.end(), string("") );
	if (pRoot && strs.size() > 1){
		strs.erase(strs.begin());
		TiXmlElement* pp = _find(pRoot,strs);
		
		if (pp == NULL){
			cerr << "SEBURO - XML Not found: " <<  s << endl; 
			return nullptr;
		}
		
		
	}
	else if (pRoot && strs.size() == 1) {
		try{
			return pRoot;
		} catch(...){
			cerr << "SEBURO - Error returning XML Value for: " << s << endl; 
			return nullptr;
		}
	}
	else {
		
		cerr << "SEBURO - XML Not found: " << s << endl; 
		return nullptr;
	}
}


/// return a std string from the iterator
std::string XMLIterator::operator* () {
	if (pElement != nullptr)
		return string(pElement->GetText());
	return string("");
}



std::string XMLIterator::Value() {
	if (pElement != nullptr)
		return string(pElement->GetText());
	return string("");
}

/// Move the iterator onwards
bool XMLIterator::Next() {
	if (pElement != nullptr)
		pElement = pElement->NextSiblingElement();
	return pElement != nullptr;
}



/**
 * Find the first (as we can have duplicates) of the path given
 */

XMLIterator XMLSettings::operator[](std::string s) {
	return Iterator(s);
}
/*
try{
			cout << ((TiXmlNode*)pp)->Value() << endl;
			return string(pp->GetText());
		} catch(...){
			cerr << "SEBURO - Error returning XML Value for: " <<  s << endl; 
			return "Failed";
		}
*/
/// Load an xml file from an absolute/relative path
bool XMLSettings::LoadFile(std::string filename){
	
	obj_.reset(new SharedObject());
	obj_->path = filename;
	obj_->doc = TiXmlDocument(filename.c_str());
	
	if (!obj_->doc.LoadFile()){
		cerr << "SEBURO - XML Failed to load " << filename << endl;
		return false;
	}
	return true;

}

/// Load an xml file from an s9 file object
bool XMLSettings::LoadFile(s9::File file){
	
	obj_.reset(new SharedObject());
	obj_->path = file.final_path();
	obj_->doc = TiXmlDocument(obj_->path.c_str());
	
	if (!obj_->doc.LoadFile()){
		cerr << "SEBURO - XML Failed to load " << obj_->path << endl;
		return false;
	}
	return true;

}


// Create an iterator from an XML path
XMLIterator XMLSettings::Iterator(std::string s){
	std::vector<std::string> strs;
	strs = SplitStringChars(s, "/: ");
	
	XMLIterator it;
	
	TiXmlElement *pRoot = obj_->doc.FirstChildElement(strs[0].c_str());
		
	if (pRoot && strs.size() > 1){
		strs.erase(strs.begin());
		TiXmlElement* pp = _find(pRoot,strs);
		it.Set(pp);
	}
	else if (pRoot && strs.size() == 1) {
		it.Set(pRoot);
	}
	else {
		cerr << "SEBURO - XML Not found: " << s << endl; 
	}
	return it;
}




