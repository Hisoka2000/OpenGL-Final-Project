#include "libs.h"

const float pi = 3.14159265;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

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
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh(GL_TRIANGLES);
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

void CreateStrip(int hVertices, int ​vVertices, float size)
{
	GLuint indices[] = {0, 4, 1, 5, 2, 6, 3, 7,
						7, 4,
						4, 8, 5, 9, 6, 10, 7, 11,
						11, 8,
						8, 12, 9, 13, 10, 14, 11, 15};

	GLfloat vertices[] = {
		0.0f,		0.0f,		0.0f,
		size,		0.0f,		0.0f,
		2 * size,	0.0f,		0.0f,
		3 * size,	0.0f,		0.0f,

		0.0f,		-size,		0.0f,
		size,		-size,		0.0f,
		2 * size,	-size,		0.0f,
		3 * size,	-size,		0.0f,

		0.0f,		-2 * size,	0.0f,
		size,		-2 * size,	0.0f,
		2 * size,	-2 * size,	0.0f,
		3 * size,	-2 * size,	0.0f,

		0.0f,		-3 * size,	0.0f,
		size,		-3 * size,	0.0f,
		2 * size,	-3 * size,	0.0f,
		3 * size,	-3 * size,	0.0f,
	};

	Mesh* obj1 = new Mesh(GL_TRIANGLE_STRIP);
	obj1->CreateMesh(vertices, indices, 48, 28);
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

	CreateStrip(4, 4, 0.5f);

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 90.0f, 3.0f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
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


		glm::mat4 model;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
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