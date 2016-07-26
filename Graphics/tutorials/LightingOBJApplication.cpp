#include "LightingOBJApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"
#include "Shader.h"
#include "Mesh.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

LightingOBJApplication::LightingOBJApplication()
	: m_camera(nullptr),
	m_shader(nullptr),
	m_modelMatrix(1) {

}

LightingOBJApplication::~LightingOBJApplication() {

}

bool LightingOBJApplication::startup() {

	// create a basic window
	createWindow("AIE Textured OBJ Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	// set up light (direction updates each frame)
	m_directionalLight.diffuse = vec3(1);
	m_directionalLight.specular = vec3(1);

	m_ambientLight = vec3(0.25f);

	// load a mesh
	m_mesh = new Mesh();
	if (m_mesh->loadObj("./models/stanford/dragon.obj", true, true) == false)
		return false;
	
	// load a shader
	m_shader = new Shader();
	if (m_shader->loadShader(GL_VERTEX_SHADER, "./shaders/phong.vert") == false) {
		printf("Vertex Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->loadShader(GL_FRAGMENT_SHADER, "./shaders/phong.frag") == false) {
		printf("Fragment Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->link() == false) {
		printf("Shader Link Error: %s\n", m_shader->getLastError());
		return false;
	}

	return true;
}

void LightingOBJApplication::shutdown() {

	// cleanup render data
	delete m_mesh;
	delete m_shader;

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool LightingOBJApplication::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	// rotate light direction
	float time = (float)glfwGetTime();
	m_directionalLight.direction = vec3(sinf(time), 0, cosf(time));

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

void LightingOBJApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	m_shader->bind();

	// create a normal matrix from the inverse model matrix (will transpose when we send to GPU)
	mat4 normalMatrix = glm::inverse(m_modelMatrix);

	// set up projection * view * model
	mat4 pvm = m_camera->getProjectionView() * m_modelMatrix;

	// bind the projection * view * model matrix
	int matUniform = m_shader->getUniform("ProjectionViewModel");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);

	// bind the model matrix
	matUniform = m_shader->getUniform("ModelMatrix");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &m_modelMatrix[0][0]);

	// bind the normal matrix (make it transposed)
	matUniform = m_shader->getUniform("NormalMatrix");
	glUniformMatrix4fv(matUniform, 1, GL_TRUE, &normalMatrix[0][0]);

	// bind light data (not using structs or uniform block for now)
	int lightUniform = m_shader->getUniform("lightDirection");
	glUniform3fv(lightUniform, 1, &m_directionalLight.direction[0]);
	lightUniform = m_shader->getUniform("Id");
	glUniform3fv(lightUniform, 1, &m_directionalLight.diffuse[0]);
	lightUniform = m_shader->getUniform("Is");
	glUniform3fv(lightUniform, 1, &m_directionalLight.specular[0]);

	// bind ambient light
	lightUniform = m_shader->getUniform("Ia");
	glUniform3fv(lightUniform, 1, &m_ambientLight[0]);

	// bind camera position for specular calculation
	int cameraUniform = m_shader->getUniform("cameraPosition");
	glUniform3fv(cameraUniform, 1, &m_camera->getTransform()[3][0]);

	// draw mesh as a triangle mesh
	m_mesh->draw(GL_TRIANGLES);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);
}