#pragma once

#include <glm.hpp>
#include <glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation);

	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;
};

