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
const static string geometry_delimiter = "##>GEOMETRY";

template<typename T, size_t N>
void ShaderVisitor::sign( ShaderClause<T, N> &c) {
	assert(false);
}

void ShaderVisitor::sign( ShaderClause<glm::mat4, 1> &c) {
	GLuint l = location(c.name.c_str());
  glUniformMatrix4fv( l, 1, GL_FALSE, glm::value_ptr(c.data));
}


template<size_t N>
void ShaderVisitor::sign( ShaderClause<float, N> &c) {
	GLuint l = location(c.name.c_str());
  glUniform1fv( l, c.size, &c.data); ///\todo test this!
}

void ShaderVisitor::sign( ShaderClause<float, 1> &c) {
	GLuint l = location(c.name.c_str());
  glUniform1f( l, c.data);
}

void ShaderVisitor::sign( ShaderClause<glm::vec4, 1> &c) {
	GLuint l = location(c.name.c_str());
  glUniform4f( l, c.data.x, c.data.y, c.data.z, c.data.w );
}


void ShaderVisitor::sign( ShaderClause<uint, 1> &c) {
	GLuint l = location(c.name.c_str());
  glUniform1ui( l, c.data );
}

void ShaderVisitor::sign( ShaderClause<int, 1> &c) {
	GLuint l = location(c.name.c_str());
  glUniform1i( l, c.data );
}




Shader::SharedObject::~SharedObject() {

	glDetachShader(program, vs); 
	glDetachShader(program, fs);
	
	if (gs != 0 ){
		glDetachShader(program, gs);
	}

}

Shader::Shader(s9::File glsl) : obj_(shared_ptr<SharedObject>(new SharedObject())) {
	string vs,fs,gs;
	string raw = textFileRead(glsl.path());
	bool error = false;

	if (!parse(raw,vs,fs,gs)) return;

	if(!createShader(GL_VERTEX_SHADER, obj_->vs, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->fs, fs)) return;

	if (gs.length() != 0){
		if(!createShader(GL_GEOMETRY_SHADER, obj_->gs, gs)) return;
	}

	createAndLink();

}
Shader::Shader(s9::File vert, s9::File frag) : obj_(shared_ptr<SharedObject>(new SharedObject())){

	string vs = textFileRead(vert.path());
	string fs = textFileRead(frag.path());

	if(!createShader(GL_VERTEX_SHADER, obj_->vs, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->fs, fs)) return;

	createAndLink();
}


Shader::Shader(s9::File vert, s9::File frag, s9::File geom) : obj_(shared_ptr<SharedObject>(new SharedObject())) {
	string vs = textFileRead(vert.path());
	string fs = textFileRead(frag.path());
	string gs = textFileRead(geom.path());

	if(!createShader(GL_VERTEX_SHADER, obj_->vs, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->fs, fs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->gs, gs)) return;

	createAndLink();
}


Shader::Shader(std::string glsl_string)  : obj_(shared_ptr<SharedObject>(new SharedObject()))  {
	string vs,fs,gs;
	bool error = false;

	if (!parse(glsl_string,vs,fs,gs)) return;

	if(!createShader(GL_VERTEX_SHADER, obj_->vs, vs)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->fs, fs)) return;

	if (gs.length() != 0){
		if(!createShader(GL_GEOMETRY_SHADER, obj_->gs, gs)) return;
	}

	createAndLink();
}

Shader::Shader(std::string vert_string, std::string frag_string)  : obj_(shared_ptr<SharedObject>(new SharedObject())) {
	if(!createShader(GL_VERTEX_SHADER, obj_->vs, vert_string)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->fs, frag_string)) return;
}


Shader::Shader(std::string vert_string, std::string frag_string, std::string geom_string)  : obj_(shared_ptr<SharedObject>(new SharedObject())) {
	if(!createShader(GL_VERTEX_SHADER, obj_->vs, vert_string)) return;
	if(!createShader(GL_FRAGMENT_SHADER, obj_->fs, frag_string)) return;
	if(!createShader(GL_GEOMETRY_SHADER, obj_->gs, geom_string)) return;
}


/**
 * Given a single GLSL file, read this in and split into its two
 * or three shader components
 */

bool Shader::parse(string &glsl, string &vs, string &fs, string &gs){

	size_t fpos = glsl.find(fragment_delimiter);
	size_t vpos = glsl.find(vertex_delimiter);
	size_t gpos = glsl.find(geometry_delimiter);

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

	positions.push_back(glsl.length()-1);

	std::sort (positions.begin(), positions.end());

	// Read the first chunk
	if (positions[0] == vpos) {
		size_t s = vpos + vertex_delimiter.length();
		vs = glsl.substr(s, positions[1] - s);
	}
	if (positions[0] == fpos) {
		size_t s = fpos + fragment_delimiter.length();
		fs = glsl.substr(s, positions[1] - s);
	}
	if (positions.size() == 4){
		if (positions[0] == gpos) {
			size_t s = gpos + geometry_delimiter.length();
			gs = glsl.substr(s, positions[1] - s);
		}
	}

	// Read the second chunk
	if (positions[1] == vpos) {
		size_t s = vpos + vertex_delimiter.length();
		vs = glsl.substr(s, positions[2] - s);
	}
	if (positions[1] == fpos) {
		size_t s = fpos + fragment_delimiter.length();
		fs = glsl.substr(s, positions[2] - s);
	}
	if (positions.size() == 4){
		if (positions[1] == gpos) {
			size_t s = gpos + geometry_delimiter.length();
			gs = glsl.substr(s, positions[2] - s);
		}
	}

	// If there is a third chunk, read that too!
	if (positions.size() == 4){
		if (positions[2] == vpos) {
			size_t s = vpos + vertex_delimiter.length();
			vs = glsl.substr(s, positions[3] - s);
		}
		if (positions[2] == fpos) {
			size_t s = fpos + fragment_delimiter.length();
			fs = glsl.substr(s, positions[3] - s);
		}
		if (positions[2] == gpos) {
			size_t s = gpos + geometry_delimiter.length();
			gs = glsl.substr(s, positions[3] - s);
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
	obj_->program = glCreateProgram();

	glAttachShader(obj_->program,obj_->vs);
	glAttachShader(obj_->program,obj_->fs);

	if (obj_->gs != 0)
		glAttachShader(obj_->program, obj_->gs);

	glLinkProgram(obj_->program);

	int isLinked;
	int maxLength;
	char* shaderProgramInfoLog;

	glGetProgramiv(obj_->program, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == false) {
		glGetProgramiv(obj_->program, GL_INFO_LOG_LENGTH, &maxLength);
		shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(obj_->program, maxLength, &maxLength, shaderProgramInfoLog);
		cerr << "SEBURO Shader Program Error - Could not Link. " << shaderProgramInfoLog << endl;
		delete[] shaderProgramInfoLog;
		return false;
	}

	return true;

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