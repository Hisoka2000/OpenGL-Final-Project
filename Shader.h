#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode, const char* geometryCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();

	GLuint GetUvScrollLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, 
		uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection,
		uniformUvScroll;

	void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

