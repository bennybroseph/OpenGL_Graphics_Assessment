#include "TexturingApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"
#include "Shader.h"

#include <stb_image.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

TexturingApplication::TexturingApplication()
	: m_camera(nullptr),
	m_shader(nullptr) {

}

TexturingApplication::~TexturingApplication() {

}

bool TexturingApplication::startup() {

	// create a basic window
	createWindow("AIE Textured OBJ Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	// load an image
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load("./textures/crate.png",
									&imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// we can use format GL_RGB because the texture only has RGB channels, no alpha
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
				 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	// set up data for a quad using just array of floats
	// first 4 are position, next 2 are texture coordinates
	float vertexData[] = {
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1, 1,
		5, 0, -5, 1, 1, 0,
		-5, 0, -5, 1, 0, 0,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
				 vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
				 indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	// using a shader that stores UVs in layout location 3
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// load a shader
	m_shader = new Shader();
	if (m_shader->loadShader(GL_VERTEX_SHADER, "./shaders/textured.vert") == false) {
		printf("Vertex Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->loadShader(GL_FRAGMENT_SHADER, "./shaders/textured.frag") == false) {
		printf("Fragment Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->link() == false) {
		printf("Shader Link Error: %s\n", m_shader->getLastError());
		return false;
	}

	return true;
}

void TexturingApplication::shutdown() {

	// cleanup render data
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteTextures(1, &m_texture);

	delete m_shader;

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TexturingApplication::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	// clear the gizmos and add a transform and grid
	Gizmos::clear();

	vec4 white(1, 1, 1, 0);
	vec4 black(0, 0, 0, 1);

	// for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	Gizmos::addTransform(mat4(1));

	// return true, else the application closes
	return true;
}

void TexturingApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	m_shader->bind();

	// bind the projection * view matrix (no world for now)
	int matUniform = m_shader->getUniform("ProjectionViewModel");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &m_camera->getProjectionView()[0][0]);

	// bind texture and set texture slot 0
	int texUniform = m_shader->getUniform("diffuseTexture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// draw mesh as a triangle mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);
}