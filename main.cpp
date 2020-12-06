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

void CreateStrip()
{
	GLuint indices[] = { 0, 4, 1, 5, 2, 6, 3, 7, 0, 4};

	GLfloat vertices[] = {
		-0.5f,  0.5f, 0.0f,
		 -0.5f,  -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,

		-0.5f,  0.5f, -0.5f,
		 -0.5f,  -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, 0.5f, -0.5f,
	};

	Mesh* obj1 = new Mesh(GL_TRIANGLE_STRIP);
	obj1->CreateMesh(vertices, indices, 24, 10);
	meshList.push_back(obj1);
}

void CreateFanCircle(GLfloat xCenter, GLfloat yCenter, GLfloat zCenter, GLfloat radius, GLint numberOfSectors)
{
	int numberOfVertices = numberOfSectors + 2;

	GLfloat twicePi = 2.0f * pi;

	std::vector<GLfloat> circleVerticesX(numberOfVertices);
	std::vector<GLfloat> circleVerticesY(numberOfVertices);
	std::vector<GLfloat> circleVerticesZ(numberOfVertices);

	circleVerticesX[0] = xCenter;
	circleVerticesY[0] = yCenter;
	circleVerticesZ[0] = zCenter;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = xCenter + (radius * cos(i * twicePi / numberOfSectors));
		circleVerticesY[i] = yCenter + (radius * sin(i * twicePi / numberOfSectors));
		circleVerticesZ[i] = zCenter;
	}

	GLfloat* vertices = new GLfloat[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		vertices[i * 3] = circleVerticesX[i];
		vertices[(i * 3) + 1] = circleVerticesY[i];
		vertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	GLuint* indices = new GLuint[numberOfVertices];
	for (int i = 0; i < numberOfVertices; i++)
	{
		indices[i] = i;
	}

	Mesh* obj1 = new Mesh(GL_TRIANGLE_FAN);
	obj1->CreateMesh(vertices, indices, numberOfVertices * 3, numberOfVertices);
	meshList.push_back(obj1);
}

void CreateCylinder(GLfloat xCenter, GLfloat yCenter, GLfloat zCenter, GLfloat radius, GLint numberOfSectors)
{
	int numberOfVertices = numberOfSectors + 2;

	GLfloat twicePi = 2.0f * pi;

	std::vector<GLfloat> circleVerticesX(numberOfVertices);
	std::vector<GLfloat> circleVerticesY(numberOfVertices);
	std::vector<GLfloat> circleVerticesZ(numberOfVertices);

	circleVerticesX[0] = xCenter;
	circleVerticesY[0] = yCenter;
	circleVerticesZ[0] = zCenter;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = xCenter + (radius * cos(i * twicePi / numberOfSectors));
		circleVerticesY[i] = yCenter + (radius * sin(i * twicePi / numberOfSectors));
		circleVerticesZ[i] = zCenter;
	}

	GLfloat* vertices = new GLfloat[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		vertices[i * 3] = circleVerticesX[i];
		vertices[(i * 3) + 1] = circleVerticesY[i];
		vertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	GLuint* indices = new GLuint[numberOfVertices];
	for (int i = 0; i < numberOfVertices; i++)
	{
		indices[i] = i;
	}

	Mesh* obj1 = new Mesh(GL_TRIANGLE_FAN);
	obj1->CreateMesh(vertices, indices, numberOfVertices * 3, numberOfVertices);
	meshList.push_back(obj1);

	GLfloat* stripVertices = new GLfloat[(numberOfVertices - 1) * 3 * 2];

	for (int i = 0; i < (numberOfVertices - 1) * 3; i+=3)
	{
		stripVertices[i] = vertices[i + 3];
		stripVertices[i + 1] = vertices[i + 4];
		stripVertices[i + 2] = vertices[i + 5];
	}

	for (int i = 0; i < numberOfVertices * 3; i+=3)
	{
		vertices[i + 2] -= 1.0f;
	}

	Mesh* obj2 = new Mesh(GL_TRIANGLE_FAN);
	obj2->CreateMesh(vertices, indices, numberOfVertices * 3, numberOfVertices);
	meshList.push_back(obj2);


	int index = 0;
	for (int i = (numberOfVertices - 1) * 3; i < (numberOfVertices - 1) * 3 * 2; i++)
	{
		stripVertices[i] = vertices[index + 3];
		index++;
	}

	GLuint* stripIndices = new GLuint[(numberOfVertices - 1) * 2];
	int number = 0;
	for (int i = 0; i < (numberOfVertices - 1) * 2; i += 2)
	{
		stripIndices[i] = number;
		stripIndices[i + 1] = (numberOfVertices - 1) + number;
		number++;
	}

	Mesh* obj3 = new Mesh(GL_TRIANGLE_STRIP);
	obj3->CreateMesh(stripVertices, stripIndices, numberOfVertices * 2 * 3 - 6, numberOfVertices * 2 - 2);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	//Taking Variables
	int numberOfSectors = 100;
	float radius = 0.5f;
	float modelRotation = 90.0f;
	std::cout << "If you want you can press 'z' inside the window to switch to wireframe mode \n";
	std::cout << "Type in the number of sectors for the cylinder: ";
	std::cin >> numberOfSectors;
	std::cout << "Type in the radius for the base of the cylinder: ";
	std::cin >> radius;

	mainWindow = Window(1600, 1200);
	mainWindow.Initialise();

	CreateCylinder(0.0f, 0.0f, 0.0f, radius, numberOfSectors);

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 90.0f, 3.0f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		/*rotation += 1;
		if (rotation >= 360)
		{
			rotation = 0.0f;
		}*/

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
		model = glm::rotate(model, modelRotation * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, modelRotation * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, modelRotation * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}