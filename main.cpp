#include "libs.h"

const float pi = 3.14159265;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture waterTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//Handle Input Time
GLfloat inputUpdateTime = 0.0f;
//Wireframe vs Solid
bool wireframe = false;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void keyInputUpdate(GLfloat& inputUpdateTime)
{
	//If enough time has passed to read a key input
	if (inputUpdateTime > 0.1f)
	{
		// Switch from wireframe mode to solid.
		if (mainWindow.getKeys()[GLFW_KEY_Z])
		{
			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				wireframe = false;
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				wireframe = true;
			}
		}

		inputUpdateTime = 0.0f;
	}
}

void CreateTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,		0.0f,	0.0f,
		 1.0f, -1.0f, 1.0f,		0.5f,	0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f,	0.0f,
		 0.0f, 1.0f, 0.0f,		0.5f,	1.0f
	};

	Mesh* obj1 = new Mesh(GL_TRIANGLES);
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);
}

void CreateStrip(int hVertices, int ​vVertices, float size)
{
	/*GLuint indices[] = {0, 4, 1, 5, 2, 6, 3, 7,
						7, 4,
						4, 8, 5, 9, 6, 10, 7, 11,
						11, 8,
						8, 12, 9, 13, 10, 14, 11, 15};*/

	GLuint* indices = new GLuint[2 * hVertices * (​vVertices - 1) + 2 * (​vVertices - 2)];

	//Setting the odd numbered indices
	int number = 0;
	int hIndex = 0;
	for (int i = 0; i < 2 * hVertices * (​vVertices - 1) + 2 * (​vVertices - 2); i+=2)
	{
		indices[i] = number;
		number++;

		if (hIndex == 2 * hVertices - 2)
		{
			i += 2;
			hIndex = 0;
			continue;
		}

		hIndex += 2;
	}

	//Setting the even numbered indices
	number = hVertices;
	hIndex = 1;
	for (int i = 1; i < 2 * hVertices * (​vVertices - 1) + 2 * (​vVertices - 2); i += 2)
	{
		indices[i] = number;
		number++;

		if (hIndex == 2 * hVertices - 1)
		{
			i += 2;
			hIndex = 1;
			continue;
		}

		hIndex += 2;
	}

	//Setting the connecting indices
	number = 0;
	for (int i = hVertices * 2; i < 2 * hVertices * (​vVertices - 1) + 2 * (​vVertices - 2); i += hVertices * 2 + 2)
	{
		indices[i] = hVertices * 2 - 1 + (number * ​vVertices);
		indices[i + 1] = hVertices + (number * ​vVertices);
		number++;
	}

	//GLfloat vertices[] = {
	//	//x			y			z			u		v
	//	0.0f,		0.0f,		0.0f,		0.0f,	1.0f,
	//	size,		0.0f,		0.0f,		0.33f,	1.0f,
	//	2 * size,	0.0f,		0.0f,		0.66f,	1.0f,
	//	3 * size,	0.0f,		0.0f,		1.0f,	1.0f,

	//	0.0f,		-size,		0.0f,		0.0f,	0.66f,
	//	size,		-size,		0.0f,		0.33f,	0.66f,
	//	2 * size,	-size,		0.0f,		0.66f,	0.66f,
	//	3 * size,	-size,		0.0f,		1.0f,	0.66f,

	//	0.0f,		-2 * size,	0.0f,		0.0f,	0.33f,
	//	size,		-2 * size,	0.0f,		0.33f,	0.33f,
	//	2 * size,	-2 * size,	0.0f,		0.66f,	0.33f,
	//	3 * size,	-2 * size,	0.0f,		1.0f,	0.33f,

	//	0.0f,		-3 * size,	0.0f,		0.0f,	0.0f,
	//	size,		-3 * size,	0.0f,		0.33f,	0.0f,
	//	2 * size,	-3 * size,	0.0f,		0.66f,	0.0f,
	//	3 * size,	-3 * size,	0.0f,		1.0f,	0.0f
	//};

	GLfloat* vertices = new GLfloat[hVertices * ​vVertices * 5];
	int xMultiplier = 0;
	int yMultiplier = 0;
	float uIncrement = 0.0f;
	float vIncrement = 1.0f;
	for (int i = 0; i < hVertices * ​vVertices * 5; i += 5)
	{
		vertices[i] = xMultiplier * size;
		vertices[i + 1] = yMultiplier * (-size);
		vertices[i + 2] = 0;
		vertices[i + 3] = uIncrement;
		vertices[i + 4] = vIncrement;

		xMultiplier++;
		uIncrement += (1.0f / (hVertices - 1));

		if (xMultiplier == hVertices)
		{
			xMultiplier = 0;
			yMultiplier++;
			uIncrement = 0.0f;
			vIncrement -= (1.0f / (​vVertices - 1));
		}
	}

	Mesh* obj1 = new Mesh(GL_TRIANGLE_STRIP);
	obj1->CreateMesh(vertices, indices, hVertices * ​vVertices * 5, 2 * hVertices * (​vVertices - 1) + 2 * (​vVertices - 2));
	meshList.push_back(obj1);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1600, 1200);
	mainWindow.Initialise();

	CreateStrip(6, 6, 0.5f);

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 90.0f, 3.0f, 0.2f);

	waterTexture = Texture((char*)("Textures/water.png"));
	waterTexture.LoadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformUvScroll = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		inputUpdateTime += deltaTime;

		// Get and Handle user input events
		glfwPollEvents();

		//Update stuff according to key presses (for example switching to wireframe mode with Z)
		keyInputUpdate(inputUpdateTime);

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.15f, 0.15f, 0.15f, 0.15f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		uniformUvScroll = shaderList[0].GetUvScrollLocation();
		glUniform1f(uniformUvScroll, glfwGetTime() / 2.5);

		glm::mat4 model;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		waterTexture.UseTexture();
		meshList[0]->RenderMesh();

		/*model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, modelRotation * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}