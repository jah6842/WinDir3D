#ifndef _MYLOADER_H
#define _MYLOADER_H

// Author: Justin Hoover
// Derived from https://www.opengl.org/wiki/Program_Pipeline_Object#Program_pipelines

#include <fstream>
#include <iostream>
#include <GL\glew.h>

static char* LoadShaderFile(const char* file)
{
	// Load the file
	std::ifstream infile(file, std::ios::binary | std::ios::in);
	if(!infile){
		std::cerr << "Failed Loading Shader: " << file << std::endl;
	    exit( EXIT_FAILURE );
	}

	// Find the size
	infile.seekg(0, std::ios::end);
	int size = static_cast<int>(infile.tellg());
	infile.seekg(0, std::ios::beg);

	// Fill the buffer and null terminate
	char* data = new char[size + 1];
	infile.read(data, size);
	data[size] = '\0';

	// Return the buffer
	return data;
}

static GLuint CreateShaderProgram(const char* vshader, const char* fshader){
	// Load the shaders from file
	const GLchar* vsource = (const GLchar*) LoadShaderFile(vshader);
	const GLchar* fsource = (const GLchar*) LoadShaderFile(fshader);

	//Send the vertex shader source code to GL
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsource, 0);
 
	//Compile the vertex shader
	glCompileShader(vertexShader);
	
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
 
		//The maxLength includes the NULL character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		printf(infoLog);
		delete[] infoLog;
		std::cout << "sdfds";
 
		//We don't need the shader anymore.
		glDeleteShader(vertexShader);
	}

	//Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
	//Send the fragment shader source code to GL
	glShaderSource(fragmentShader, 1, &fsource, 0);
 
	//Compile the fragment shader
	glCompileShader(fragmentShader);
 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
 
		//The maxLength includes the NULL character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
		printf(infoLog);
		delete[] infoLog;
 
		//We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		//Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);
	}
 
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	GLuint program = glCreateProgram();
 
	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
 
	//Link our program
	glLinkProgram(program);
 
	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
 
		//The maxLength includes the NULL character
		char* infoLog = new char[maxLength];
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		printf(infoLog);
		delete[] infoLog;
 
		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	delete[] vsource;
	delete[] fsource;

	return program;
}

#endif // _MYLOADER_H