/**
* @brief S9 XML Reading / Writing Classes
* @file camera.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/s9xml.hpp"

using namespace std;
using namespace boost;
using namespace s9; 


TiXmlElement* _find(TiXmlElement *p, std::vector<std::string> s){
	p = p->FirstChildElement(s[0].c_str());
	s.erase(s.begin());
	if (s.size() > 0 && p) {
		return _find(p,s);
	}
	return p;
}


std::string find(std::vector<std::string> strs, TiXmlElement *pRoot){
	
	string s = accumulate( strs.begin(), strs.end(), string("") );
	if (pRoot && strs.size() > 1){
		strs.erase(strs.begin());
		TiXmlElement* pp = _find(pRoot,strs);
		
		if (pp == NULL){
			cerr << "OpenGLCourse - XML Not found: " <<  s << endl; 
			return "Failed";
		}
		
		try{
			cout << ((TiXmlNode*)pp)->Value() << endl;
			return string(pp->GetText());
		} catch(...){
			cerr << "OpenGLCourse - Error returning XML Value for: " <<  s << endl; 
			return "Failed";
		}
	}
	else if (pRoot && strs.size() == 1) {
		try{
			return string(pRoot->GetText());
		} catch(...){
			cerr << "OpenGLCourse - Error returning XML Value for: " << s << endl; 
			return "Failed";
		}
	}
	else {
		
		cerr << "OpenGLCourse - XML Not found: " << s << endl; 
		return "Failed";
	}
}

std::string XMLIterator::operator* () {
	return string(pElement->GetText());
}


bool XMLIterator::next() {
	if (pElement != NULL)
		pElement = pElement->NextSiblingElement();
	return pElement != NULL;
}



std::string XMLIterator::operator[](const char *s) {
	//split string via '/' into levels
	string ss(s);
	std::vector<std::string> strs;
	boost::split(strs, ss, boost::is_any_of("/: "));
	TiXmlElement *pRoot = pElement->FirstChildElement(strs[0].c_str());
	return find(strs,pRoot);
	
}

/*
 * Find the first (as we can have duplicates) of the path given
 */

std::string XMLSettings::operator[](std::string s) {

	// todo - be careful here if we ever add push and pop to the stack
	// as the cache will need to keep the FULL path

	map< string,string>::iterator it;
	it = mObj->mCache.find(s);
	if (it == mObj->mCache.end()){
		//split string via '/' into levels
		std::vector<std::string> strs;
		boost::split(strs, s, boost::is_any_of("/: "));

		TiXmlElement *pRoot = mObj->mDoc->FirstChildElement(strs[0].c_str());
		string result = find(strs,pRoot);
		mObj->mCache[s] = result;
		return result;
	}
	return it->second;
}

bool XMLSettings::loadFile(std::string filename){
	
	mObj.reset(new SharedObj());
	mObj->mFilename = filename;

	mObj->mDoc.reset(new TiXmlDocument(filename.c_str()));
	if (!mObj->mDoc->LoadFile()){
		cerr << "OpenGLCourse - XML Failed to load " << filename << endl;
		return false;
	}
	return true;

}


XMLIterator XMLSettings::iterator(std::string s){
	std::vector<std::string> strs;
	boost::split(strs, s, boost::is_any_of("/: "));
	
	XMLIterator it;
	
	TiXmlElement *pRoot = mObj->mDoc->FirstChildElement(strs[0].c_str());
		
	if (pRoot && strs.size() > 1){
		strs.erase(strs.begin());
		TiXmlElement* pp = _find(pRoot,strs);
		it.set(pp);
	}
	else if (pRoot && strs.size() == 1) {
		it.set(pRoot);
	}
	else {
		cerr << "OpenGLCourse - XML Not found: " << s << endl; 
	}
	return it;
}




