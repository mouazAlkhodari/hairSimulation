#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "Renderer.h"
using namespace std;
class Shader {
private:
	unsigned int m_RendererID;
public:
	Shader(const string & vertexShader, const string & fragmentShader);
	~Shader();

	void use();
	void unUse();

	void SetUniform4f(const string& name, float v1, float v2, float v3, float v4);
private:
	unsigned int createProgram(const string & vertexShader, const string & fragmentShader);
	unsigned int compileShader(unsigned int type, const string & source);
	string readFile(const string& path);
	int GetUniformLocation(string const& name);  
};

Shader::Shader(const string & vertexShaderPath, const string & fragmentShaderPath)
	:m_RendererID(0){
	string vertexShader = readFile(vertexShaderPath);
	string fragmentShader = readFile(fragmentShaderPath);
	m_RendererID = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	glDeleteShader(m_RendererID);
}
string Shader::readFile(const string& path) {
	ifstream fstream(path);
	stringstream ss;
	ss << fstream.rdbuf();
	return ss.str();
}

unsigned int Shader::compileShader(unsigned int type, const string & source) {
	// Creating Shaders 
	unsigned int shader = glCreateShader(type);

	// bind the Source to the Shader
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);

	//Compile The shader and make sure every thing is alright
	glCompileShader(shader);

	// handling Error
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		cout << "failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader" << endl;
		cout << message << endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
unsigned int Shader::createProgram(const string & vertexShader, const string & fragmentShader) {
	// creating shader
	unsigned int program = glCreateProgram();

	// creating the shaders and compiling it 
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// binding shaders to the program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// link program with GPU and make sure that work properly
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
void Shader::use()
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::unUse()
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const string& name, float v1, float v2, float v3, float v4)
{
	GLCall(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}

int Shader::GetUniformLocation(string const& name)
{
	GLCall(int loaction = glGetUniformLocation(m_RendererID, name.c_str()));
	if (loaction == -1) {
		cout << "Warning uniform '" << name << "' dose not exists!" << endl;
	}
	return loaction;

}
