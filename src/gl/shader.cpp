/**
* @brief Shader Classes
* @file shader.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "gl/shader.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 
using namespace s9::gl;


std::string textFileRead(std::string filename) {
	string line;
	string rval;
	ifstream myfile (filename.c_str());
	if (myfile.is_open()){
		while ( myfile.good() )	{
			getline (myfile,line);
			rval += line +"\n";
		}
		myfile.close();
	}

	else cerr << "S9Gear - Unable to open shader file " << filename << endl;

	return rval;
}



void Shader::load(std::string vert, std::string frag) {
	
	int isCompiled_VS, isCompiled_FS;
	
	int maxLength;
	int IsLinked;
	char *vertexInfoLog;
	char *fragmentInfoLog;
	char *shaderProgramInfoLog;
	
	mVS = glCreateShader(GL_VERTEX_SHADER);
	mFS = glCreateShader(GL_FRAGMENT_SHADER);	

	string sv = textFileRead(vert);
	string sf = textFileRead(frag);

	const char * vv = sv.c_str();
	const char * ff = sf.c_str();

	glShaderSource(mVS, 1, &vv,NULL);
	glShaderSource(mFS, 1, &ff,NULL);

	glCompileShader(mVS);
	
	glGetShaderiv(mVS, GL_COMPILE_STATUS, &isCompiled_VS);
	if(isCompiled_VS == false) {
		glGetShaderiv(mVS, GL_INFO_LOG_LENGTH, &maxLength);
 
		vertexInfoLog = new char[maxLength];
		glGetShaderInfoLog(mVS, maxLength, &maxLength, vertexInfoLog);
 
		cerr << "S9Gear - Vertex Shader Error in " <<  vert << " - "<< vertexInfoLog << endl;
 
		delete [] vertexInfoLog;
		return;
	}
	
	glCompileShader(mFS);
	glGetShaderiv(mFS, GL_COMPILE_STATUS, &isCompiled_FS);
	if(isCompiled_FS == false) {
		glGetShaderiv(mFS, GL_INFO_LOG_LENGTH, &maxLength);
		fragmentInfoLog = new char[maxLength];
		glGetShaderInfoLog(mFS, maxLength, &maxLength, fragmentInfoLog);
		cerr << "S9Gear - Fragment Shader Error in " << frag << " - " << fragmentInfoLog << endl;
		
		delete [] fragmentInfoLog;
		return;
	}
	
	mProgram = glCreateProgram();

	glAttachShader(mProgram,mVS);
	glAttachShader(mProgram,mFS);
	glLinkProgram(mProgram);
	
	glGetProgramiv(mProgram, GL_LINK_STATUS, (int *)&IsLinked);
	if(IsLinked == false) {
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLength);
		shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(mProgram, maxLength, &maxLength, shaderProgramInfoLog);
		cerr << "S9Gear - Shader Program Error " << shaderProgramInfoLog << endl;
		free(shaderProgramInfoLog);
		return;
	}
}
