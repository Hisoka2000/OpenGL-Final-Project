#pragma once
#include <glew.h>

class Mesh
{
public:
	Mesh(GLenum mode);

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLenum mode;
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

