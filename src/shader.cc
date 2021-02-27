#include "shader.hh"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "gfx.hh"
#include <stdio.h>
#include <stdlib.h>

const char* readFile(const char* path) {
  std::ifstream t;
  int length;
  t.open(path);                 // open input file
  t.seekg(0, std::ios::end);    // go to the end
  length = t.tellg();           // report location (this is the length)
  t.seekg(0, std::ios::beg);    // go back to the beginning
  char* buffer = (char*) malloc(length + 1);    // allocate memory for a buffer of appropriate dimension
  t.read(buffer, length);       // read the whole file into the buffer
  t.close();                    // close file handle

  // idk why but this fixes stuff? for some reason the buffer doesn't add \0
  // maybe im not understand c++ buffers
  buffer[length] = '\0';

  return buffer;
}

void Shader::load(const char* vertex_path, const char* frag_path) {
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // load as string from file
	const char* vertexSourcePointer = readFile(vertex_path);
	const char* fragmentSourcePointer = readFile(frag_path);

  //printf("v: %s\n\n", vertexSourcePointer);
  //printf("f: %s\n\n", fragmentSourcePointer);

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader: %s\n", vertex_path);
	glShaderSource(vertexShaderID, 1, (const char* const*) &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
    char* vertexShaderErrorMessage = (char*) malloc(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, vertexShaderErrorMessage);
		printf("%s\n", vertexShaderErrorMessage);
    free(vertexShaderErrorMessage);
	}

	// Compile Fragment Shader
	printf("Compiling shader: %s\n", frag_path);
	glShaderSource(fragmentShaderID, 1, (const char* const*) &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
    char* fragmentShaderErrorMessage = (char*) malloc(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, fragmentShaderErrorMessage);
		printf("%s\n", fragmentShaderErrorMessage);
    free(fragmentShaderErrorMessage);
	}

	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {
    char* programErrorMessage = (char*) malloc(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, programErrorMessage);
		printf("%s\n", programErrorMessage);
    free(programErrorMessage);
    throw std::runtime_error("Failed to compile shaders");
	}
	
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

  id = programID;
}

GLint Shader::getUniform(const char* name) {
  return glGetUniformLocation(id, name);
}

void Shader::use() {
  glUseProgram(id);
}

Shader::~Shader() {
  glDeleteProgram(id);
}
