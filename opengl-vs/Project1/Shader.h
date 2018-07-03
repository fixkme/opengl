#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	Shader(GLchar *vertexPath, GLchar *fragmentPath);
	Shader(const GLchar *vertexShaderCode, const GLchar *fragmentShaderCode, int);
	~Shader();
	//使用着色器
	void use();
	//获得着色器程序
	GLuint getProgram();
	//
	void setVec3f(GLint uniformLoc, GLfloat x, GLfloat y, GLfloat z);
	void setMat4f(GLint uniformLoc, const GLfloat *value, GLsizei count = 1, GLboolean isTranspose = GL_FALSE);
	void set1i(GLint uniformLoc, GLint value);
private:
	GLuint program;
};

#endif