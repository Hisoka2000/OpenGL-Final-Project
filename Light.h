#pragma once

#include <glm.hpp>
#include <glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
				GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

