#include "MyApplication.h"

#include <Gizmos.h>
#include "FlyCamera.h"

const static int GRID_SIZE = 10;
const static int GRID_SEPARATOR = 5;

const static unsigned int VERTEX_SIZE = 4;
const static unsigned int INDEX_BUFFER_SIZE = 4;

int MyApplication::startup()
{
	createWindow("Intro to OpenGL", 1280, 720);

	generateGrid();

	auto vsSource =
		"#version 410\n \
		 layout(location=0) in vec4 position; \
		 layout(location=1) in vec4 colour; \
		 out vec4 vColour; \
		 uniform mat4 projectionViewWorldMatrix; \
		 void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position;}";

	auto fsSource =
		"#version 410\n \
		 in vec4 vColour; \
		 out vec4 fragColor; \
		 void main() { fragColor = vColour; }";

	auto success = GL_FALSE;
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, static_cast<const char**>(&vsSource), nullptr);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, static_cast<const char**>(&fsSource), nullptr);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		auto infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		auto infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	Gizmos::create();

	//setup some camera stuff
	m_camera = new FlyCamera();

	m_sun = Planet(vec3(0, 0, 0), 1.5f, vec4(255.f / 255.f, 235.f / 255.f, 59.f / 255.f, 1.f), 1.5f);

	m_earth = Planet(vec3(5, 0, 0), 1.f, vec4(139 / 255.f, 195 / 255.f, 74 / 255.f, 1.f), 3.f);
	m_earth.transform().setParent(&m_sun.transform());

	m_moon = Planet(vec3(2, 0.5f, 0), 0.3f, vec4(0.9f, 0.9f, 0.9f, 1), 5.f);
	m_moon.transform().setParent(&m_earth.transform());

	return true;
}

void MyApplication::shutdown()
{
	//delete the camera
	Gizmos::destroy(); //cleanup the gizmos
	destroyWindow();
}

void MyApplication::parseInput()
{
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
	{
		m_isRunning = false;
		return;
	}

	if (Input::getKey(GLFW_KEY_F1) == GLFW_PRESS)
		m_shouldDrawGrid = !m_shouldDrawGrid;

	if (Input::getKey(GLFW_KEY_1, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL) >= GLFW_PRESS)
		m_sun.transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_2) >= GLFW_PRESS)
		m_earth.transform().translate(vec3(0.f, 1.f * m_deltaTime, 0.f));

	if (Input::getKey(GLFW_KEY_3) >= GLFW_PRESS)
		m_moon.transform().setPosition(vec3(0, 5, 0));
}

void MyApplication::update()
{
	m_sun.update(m_deltaTime);
	m_earth.update(m_deltaTime);
	m_moon.update(m_deltaTime);

	m_camera->update(m_deltaTime);
}

void MyApplication::lateUpdate() { }

void MyApplication::draw()
{
	Gizmos::clear();

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_shouldDrawGrid)
	{
		glUseProgram(m_programID);
		unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
		glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(m_camera->getProjectionView()));

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLE_STRIP, INDEX_BUFFER_SIZE, GL_UNSIGNED_INT, nullptr);
	}

	drawSolarSystem();

	Gizmos::draw(m_camera->getProjectionView());
}

void MyApplication::drawSolarSystem()
{
	Gizmos::addSphere(
		m_sun.transform().getPosition(),
		m_sun.radius(), 25, 25,
		m_sun.colour(),
		&m_sun.transform().getWorldSpaceMatrix());

	Gizmos::addSphere(
		m_earth.transform().getPosition(),
		m_earth.radius(), 25, 25,
		m_earth.colour(),
		&m_earth.transform().getWorldSpaceMatrix());

	Gizmos::addSphere(
		m_moon.transform().getPosition(),
		m_moon.radius(), 25, 25,
		m_moon.colour(),
		&m_moon.transform().getWorldSpaceMatrix());
}

void MyApplication::generateGrid()
{
	// create vertex and index data for a quad
	Vertex vertices[4];
	unsigned int indices[4] = { 0, 2, 1, 3 };

	vertices[0].position = vec4(-5, 0, -5, 1);
	vertices[1].position = vec4(5, 0, -5, 1);
	vertices[2].position = vec4(-5, 0, 5, 1);
	vertices[3].position = vec4(5, 0, 5, 1);

	vertices[0].colour = vec4(1, 0, 0, 1);
	vertices[1].colour = vec4(0, 1, 0, 1);
	vertices[2].colour = vec4(0, 0, 1, 1);
	vertices[3].colour = vec4(1, 1, 1, 1);

	//gen vbo,ibo
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//bind and pop vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_SIZE * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//bind and pop ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_SIZE * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//setup vertex descriptors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec4)));

	//teardown unbind all handles
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
