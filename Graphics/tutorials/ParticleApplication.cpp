#include "ParticleApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"
#include "Shader.h"
#include "ParticleEmitter.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

ParticleApplication::ParticleApplication()
	: m_camera(nullptr),
	m_shader(nullptr) {

}

ParticleApplication::~ParticleApplication() {

}

bool ParticleApplication::startup() {

	// create a basic window
	createWindow("AIE Particle Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	// initialise an emitter with an interesting test
	m_emitter = new ParticleEmitter();
	m_emitter->initialise(1000, 500,
						  0.1f, 1.0f,
						  1, 5,
						  1, 0.1f,
						  vec4(1, 0, 0, 1), vec4(1, 1, 0, 1));
	// load a shader
	m_shader = new Shader();
	if (m_shader->loadShader(GL_VERTEX_SHADER, "./shaders/particle.vert") == false) {
		printf("Vertex Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->loadShader(GL_FRAGMENT_SHADER, "./shaders/particle.frag") == false) {
		printf("Fragment Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->link() == false) {
		printf("Shader Link Error: %s\n", m_shader->getLastError());
		return false;
	}

	return true;
}

void ParticleApplication::shutdown() {

	// cleanup render data
	delete m_emitter;
	delete m_shader;

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool ParticleApplication::update(float deltaTime) {
	
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	// update the emitter to face the camera and emit new particles
	m_emitter->update(deltaTime, m_camera->getTransform());

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

void ParticleApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind a basic shader
	m_shader->bind();

	// bind the camera for visibility
	int matUniform = m_shader->getUniform("ProjectionViewModel");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &m_camera->getProjectionView()[0][0]);

	// display the particles
	m_emitter->draw();
	
	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);
}