/**
* @brief Shader Classes
* @file shader.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/gl/shader.hpp"

using namespace std;
using namespace s9::gl;

/**
 * Load a shader from one single GLSL file - delimeters are the same as coffeegl
 */

const static string fragment_delimiter = "##>FRAGMENT";
const static string vertex_delimiter = "##>VERTEX";
const static string geometry_delimiter = "##>GEOM";


Shader::~Shader() {
	glDetachShader(program_, vs_); 
	glDetachShader(program_, fs_);
	
	if (gs_ != 0 ){
		glDetachShader(program_, gs_);
	}
}

/**
 * Given a single GLSL file, read this in and split into its two
 * or three shader components
 */

bool Shader::parse(string &glsl, string &vs, string &fs, string &gs){
	string raw = textFileRead(glsl);

	size_t fpos = raw.find(fragment_delimiter);
	size_t vpos = raw.find(vertex_delimiter);
	size_t gpos = raw.find(geometry_delimiter);

	std::vector<size_t> positions;

	if (fpos ==string::npos){
		cerr << "SEBURO Shader Parse Error - No Fragment Shader found." << endl;
		return false;
	} 

	positions.push_back(fpos);

	if (vpos == string::npos){
		cerr << "SEBURO Shader Parse Error - No Vertex Shader found." << endl;
		return false;
	}

	positions.push_back(vpos);

	if (gpos != string::npos) positions.push_back(gpos);

	positions.push_back(raw.length()-1);

	std::sort (positions.begin(), positions.end());

	// Read the first chunk
	if (positions[0] == vpos) {
		size_t s = vpos + vertex_delimiter.length();
		vs = raw.substr(s, positions[1] - s);
	}
	if (positions[0] == fpos) {
		size_t s = fpos + fragment_delimiter.length();
		fs = raw.substr(s, positions[1] - s);
	}
	if (positions.size() == 4){
		if (positions[0] == gpos) {
			size_t s = gpos + geometry_delimiter.length();
			gs = raw.substr(s, positions[1] - s);
		}
	}

	// Read the second chunk
	if (positions[1] == vpos) {
		size_t s = vpos + vertex_delimiter.length();
		vs = raw.substr(s, positions[2] - s);
	}
	if (positions[1] == fpos) {
		size_t s = fpos + fragment_delimiter.length();
		fs = raw.substr(s, positions[2] - s);
	}
	if (positions.size() == 4){
		if (positions[1] == gpos) {
			size_t s = gpos + geometry_delimiter.length();
			gs = raw.substr(s, positions[2] - s);
		}
	}

	// If there is a third chunk, read that too!
	if (positions.size() == 4){
		if (positions[2] == vpos) {
			size_t s = vpos + vertex_delimiter.length();
			vs = raw.substr(s, positions[3] - s);
		}
		if (positions[2] == fpos) {
			size_t s = fpos + fragment_delimiter.length();
			fs = raw.substr(s, positions[3] - s);
		}
		if (positions[2] == gpos) {
			size_t s = gpos + geometry_delimiter.length();
			gs = raw.substr(s, positions[3] - s);
		}
	}
	return true;
}


bool Shader::createShader(GLenum type, GLuint &handle, string &data){
	handle = glCreateShader(type);
	const char * tdata = data.c_str();
	glShaderSource(handle, 1, &tdata, NULL);
	glCompileShader(handle);

	int is_compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &is_compiled);

	if (is_compiled == false) {
		switch(type){
			case GL_VERTEX_SHADER :
				cerr << "SEBURO Shader Error - Could not compile Vertex Shader.";
			break;
			case GL_FRAGMENT_SHADER :
				cerr << "SEBURO Shader Error - Could not compile Fragment Shader.";
			break;

			case GL_GEOMETRY_SHADER :
				cerr << "SEBURO Shader Error - Could not compile Geometry Shader.";
			break;

			default:
				cerr << "SEBURO Shader Error - Unsupported shader type." << endl;
				return false;
			break;
		}

		int maxLength;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
 
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(handle, maxLength, &maxLength, infoLog);

		cerr << endl << infoLog << endl;

		delete [] infoLog;
		return false;

	}
	return true;

}

bool Shader::createAndLink() {
	program_ = glCreateProgram();

	glAttachShader(program_,vs_);
	glAttachShader(program_,fs_);

	if (gs_ != 0)
		glAttachShader(program_, gs_);

	glLinkProgram(program_);

	int isLinked;
	int maxLength;
	char* shaderProgramInfoLog;

	glGetProgramiv(program_, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == false) {
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxLength);
		shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(program_, maxLength, &maxLength, shaderProgramInfoLog);
		cerr << "SEBURO Shader Program Error - Could not Link. " << shaderProgramInfoLog << endl;
		delete[] shaderProgramInfoLog;
		return false;
	}

	return true;

}

void Shader::load(string glsl) {
	string vs,fs,gs;
	string raw = textFileRead(glsl);
	bool error = false;

	if (!parse(raw,vs,fs,gs)) return;

	if(!createShader(GL_VERTEX_SHADER, vs_, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, fs_, fs)) return;

	if (gs.length() != 0){
		if(!createShader(GL_GEOMETRY_SHADER, gs_, gs)) return;
	}

	createAndLink();
}


/**
 * Load shader from vertex and fragment file
 */

void Shader::load(string vert, string frag) {
	
	string vs = textFileRead(vert);
	string fs = textFileRead(frag);

	if(!createShader(GL_VERTEX_SHADER, vs_, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, fs_, fs)) return;

	createAndLink();

}

/**
 * Load shader from vertex, fragment and geometry file
 */


void Shader::load(string vert, string frag, string geom) {
	
	string vs = textFileRead(vert);
	string fs = textFileRead(frag);
	string gs = textFileRead(geom);

	if(!createShader(GL_VERTEX_SHADER, vs_, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, fs_, fs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, gs_, gs)) return;

	createAndLink();

}


/*
 * Fluent Style interface - Overloaded setters for uniforms
 */

Shader& Shader::s(const char * name, glm::vec3 v) {
	GLuint l = location(name);
	glUniform3f(l,v.x,v.y,v.z);
	return *this;
}

Shader& Shader::s(const char * name, glm::vec4 v) {
	GLuint l = location(name);
	glUniform4f(l,v.x,v.y,v.z,v.w);
	return *this;

}

Shader& Shader::s(const char * name, glm::mat4 v) {
	GLuint l = location(name);
	glUniformMatrix4fv(	l, 1, GL_FALSE, glm::value_ptr(v)); 
	return *this;
}


Shader& Shader::s(const char * name, float f) {
	GLuint l = location(name);
	glUniform1f(l,f);
	return *this;
}

Shader& Shader::s(const char * name, int i){
	GLuint l = location(name);
	glUniform1i(l,i);
	return *this;
}