#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode, const char* geometryCode)
{
	CompileShader(vertexCode, fragmentCode, geometryCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	std::string geometryString = ReadFile(geometryLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	const char* geometryCode = geometryString.c_str();

	CompileShader(vertexCode, fragmentCode, geometryCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File does not exist in specified location.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) 
	{
		printf("Shader Program Creation FAILED \n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Program Linking FAILED: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Program Validation FAILED: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.colour");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");

	uniformUvScroll = glGetUniformLocation(shaderID, "uvScroll");
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientColourLocation()
{
	return uniformAmbientColor;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirection;
}

GLuint Shader::GetUvScrollLocation()
{
	return uniformUvScroll;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) 
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("%d Shader Compilation FAILED: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	ClearShader();
}