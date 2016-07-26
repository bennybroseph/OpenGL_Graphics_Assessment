#include "LightingSphereApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"
#include "Shader.h"
#include "Mesh.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

LightingSphereApplication::LightingSphereApplication()
	: m_camera(nullptr),
	m_shader(nullptr),
	m_modelMatrix(1) {

}

LightingSphereApplication::~LightingSphereApplication() {

}

bool LightingSphereApplication::startup() {

	// create a basic window
	createWindow("AIE Lit Sphere Application", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	// set up light (direction updates each frame)
	m_directionalLight.diffuse = vec3(1);
	m_directionalLight.specular = vec3(1);
	m_ambientLight = vec3(0.25f);

	// set up material
	m_material.diffuse = vec3(1);
	m_material.ambient = vec3(1);
	m_material.specular = vec3(1);
	m_material.specularPower = 64;

	// generate a sphere with radius 5
	generateSphere(32, 32, m_vao, m_vbo, m_ibo, m_indexCount);
	m_modelMatrix = glm::scale(vec3(5));
		
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

void LightingSphereApplication::shutdown() {

	// cleanup render data
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);

	delete m_shader;

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool LightingSphereApplication::update(float deltaTime) {
	
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

void LightingSphereApplication::draw() {

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

	// bind material
	int materialUniform = m_shader->getUniform("Ka");
	glUniform3fv(materialUniform, 1, &m_material.ambient[0]);

	materialUniform = m_shader->getUniform("Kd");
	glUniform3fv(materialUniform, 1, &m_material.diffuse[0]);

	materialUniform = m_shader->getUniform("Ks");
	glUniform3fv(materialUniform, 1, &m_material.specular[0]);

	materialUniform = m_shader->getUniform("specularPower");
	glUniform1f(materialUniform, m_material.specularPower);

	// draw mesh as a triangle mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(guiMatrix);
}

void LightingSphereApplication::generateSphere(unsigned int segments, unsigned int rings,
											   unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
											   unsigned int& indexCount) {

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	AIEVertex* vertices = new AIEVertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = glm::pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / segments;

	AIEVertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring) {
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex) {
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = vec4(x0, y0, z0, 0);

			vertex->tangent = vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			// not a part of the AIEVertex, but this is how w generate bitangents
			//vertex->bitangent = vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			vertex->texcoord = vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i) {
		for (unsigned j = 0; j < segments; ++j) {
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(AIEVertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(AIEVertex), 0);

	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(AIEVertex), (void*)(sizeof(vec4)*2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(AIEVertex), (void*)(sizeof(vec4)*3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(AIEVertex), (void*)(sizeof(vec4)*3+sizeof(vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}