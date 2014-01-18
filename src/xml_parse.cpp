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


std::string Find(std::vector<std::string> strs, TiXmlElement *pRoot){
	
	string s = accumulate( strs.begin(), strs.end(), string("") );
	if (pRoot && strs.size() > 1){
		strs.erase(strs.begin());
		TiXmlElement* pp = _find(pRoot,strs);
		
		if (pp == NULL){
			cerr << "SEBURO - XML Not found: " <<  s << endl; 
			return "Failed";
		}
		
		try{
			cout << ((TiXmlNode*)pp)->Value() << endl;
			return string(pp->GetText());
		} catch(...){
			cerr << "SEBURO - Error returning XML Value for: " <<  s << endl; 
			return "Failed";
		}
	}
	else if (pRoot && strs.size() == 1) {
		try{
			return string(pRoot->GetText());
		} catch(...){
			cerr << "SEBURO - Error returning XML Value for: " << s << endl; 
			return "Failed";
		}
	}
	else {
		
		cerr << "SEBURO - XML Not found: " << s << endl; 
		return "Failed";
	}
}


/// return a std string from the iterator
std::string XMLIterator::operator* () {
	return string(pElement->GetText());
}


/// Move the iterator onwards
bool XMLIterator::Next() {
	if (pElement != NULL)
		pElement = pElement->NextSiblingElement();
	return pElement != NULL;
}


/// Return an iterator at a position of the element passed in
std::string XMLIterator::operator[](const char *s) {
	//split_string string via '/' into levels
	string ss(s);
	std::vector<std::string> strs;
	strs = SplitStringChars(s, "/: ");
	TiXmlElement *pRoot = pElement->FirstChildElement(strs[0].c_str());
	return Find(strs,pRoot);
	
}


/**
 * Find the first (as we can have duplicates) of the path given
 */

std::string XMLSettings::operator[](std::string s) {

	// todo - be careful here if we ever add push and pop to the stack
	// as the cache will need to keep the FULL path

	map< string,string>::iterator it;
	it = obj_->cache.find(s);
	if (it == obj_->cache.end()){
		//split_string string via '/' into levels
		std::vector<std::string> strs;
		strs = SplitStringChars(s, "/: ");
		TiXmlElement *pRoot = obj_->doc->FirstChildElement(strs[0].c_str());
		string result = Find(strs,pRoot);
		obj_->cache[s] = result;
		return result;
	}
	return it->second;
}

/// Load an xml file from an absolute/relative path
bool XMLSettings::LoadFile(std::string filename){
	
	obj_.reset(new SharedObject());
	obj_->path = filename;

	obj_->doc.reset(new TiXmlDocument(filename.c_str()));
	if (!obj_->doc->LoadFile()){
		cerr << "SEBURO - XML Failed to load " << filename << endl;
		return false;
	}
	return true;

}

/// Load an xml file from an s9 file object
bool XMLSettings::LoadFile(s9::File file){
	
	obj_.reset(new SharedObject());
	obj_->path = file.final_path();

	obj_->doc.reset(new TiXmlDocument(obj_->path.c_str()));
	if (!obj_->doc->LoadFile()){
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
	
	TiXmlElement *pRoot = obj_->doc->FirstChildElement(strs[0].c_str());
		
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




