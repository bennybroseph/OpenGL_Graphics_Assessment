#include "ProceduralGenerationApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"
#include "Shader.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

ProceduralGenerationApplication::ProceduralGenerationApplication()
	: m_camera(nullptr),
	m_shader(nullptr) {
	/*
		To start with, we will be using glm’s perlin noise function to generate our noise instead of
		implementing our own. The glm function simply takes in a vec2 and returns a float between -1 and 1.
		Before just passing in our x and y we want to make some changes to them. First, we scale our point
		down, in this case by 10. This is because Perlin noise always returns 0 on integer boundaries. The
		Perlin noise function also returns a number between -1 and 1. For our height map we want a
		number between 0 and 1 so we can later multiply it by our max height. To do this we multiply by 0.5
		and then add by 0.5.
	*/
	m_perlinDimensions = 64;
	m_perlinData = new float[m_perlinDimensions * m_perlinDimensions];

	// scale helps to control the noise
	float scale = (1.0f / m_perlinDimensions) * 3;

	int octaves = 6;
	for (unsigned int x = 0; x < m_perlinDimensions; ++x) {
		for (unsigned int y = 0; y < m_perlinDimensions; ++y) {

			float amplitude = 1.f;
			float persistence = 0.3f;

			m_perlinData[x * m_perlinDimensions + y] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);

				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;

				m_perlinData[x * m_perlinDimensions + y] += perlin_sample * amplitude;

				amplitude *= persistence;
			}
		}
	}
}

ProceduralGenerationApplication::~ProceduralGenerationApplication() {
	delete m_perlinData;
}

bool ProceduralGenerationApplication::startup()
{
	// create a basic window
	createWindow("AIE OpenGL Application: Procedural Generation", 1280, 720);

	// start the gizmo system that can draw basic immediate-mode shapes
	Gizmos::create();

	// setup camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	int rows = 64;
	int cols = 64;

	// create opengl data for a grid
	Vertex* vertices = new Vertex[rows * cols];
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {

			// offset position so that the terrain is centered
			vertices[r * cols + c].position = vec4(c - cols * 0.5f, 0, r - rows * 0.5f, 1);

			// setting up UVs
			vertices[r * cols + c].texCoord = vec2(c * (1.f / cols), r * (1.f / rows));
		}
	}

	// keep track of number of indices for rendering
	m_indexCount = (rows - 1) * (cols - 1) * 6;

	unsigned int* indices = new unsigned int[m_indexCount];

	unsigned int index = 0;
	for (int r = 0; r < (rows - 1); ++r) {
		for (int c = 0; c < (cols - 1); ++c) {
			// triangle 1
			indices[index++] = r * cols + c;
			indices[index++] = (r + 1) * cols + c;
			indices[index++] = (r + 1) * cols + (c + 1);
			// triangle 2
			indices[index++] = r * cols + c;
			indices[index++] = (r + 1) * cols + (c + 1);
			indices[index++] = r * cols + (c + 1);
		}
	}

	// generate buffers
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &m_vao);

	// all changes will apply to this handle
	glBindVertexArray(m_vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// texcoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec4));
	
	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// cleanup
	delete[] vertices;
	delete[] indices;

	/*
		In order to see our generated data, we are going to create a texture,
		fill it with the noise data, and display it on our quad
	*/
	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	// bind data as float for a single channel
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 
				 m_perlinDimensions, m_perlinDimensions, 0, GL_RED, GL_FLOAT, m_perlinData);

	// enable blending else samples must be "exact" centre of texels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set wrap to stop errors at edge of noise sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// load a shader
	const char* pvs = "./shaders/perlin.vert";
	const char* pfs = "./shaders/perlin.frag";

	m_shader = new Shader();
	if (m_shader->loadShader(GL_VERTEX_SHADER, pvs) == false) {
		printf("Vertex Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	
	if (m_shader->loadShader(GL_FRAGMENT_SHADER, pfs) == false) {
		printf("Fragment Shader Error: %s\n", m_shader->getLastError());
		return false;
	}
	if (m_shader->link() == false) {
		printf("Shader Link Error: %s\n", m_shader->getLastError());
		return false;
	} 

	return true;
}
 
void ProceduralGenerationApplication::shutdown() {

	// cleanup render data
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);

	// cleanup render data
	delete m_shader;

	// cleanup camera and gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool ProceduralGenerationApplication::update(float deltaTime)
{
	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	return true;
}

void ProceduralGenerationApplication::draw()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	m_shader->bind();

	// bind the projection * view matrix (no world for now)
	int matUniform = m_shader->getUniform("ProjectionViewModel");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &m_camera->getProjectionView()[0][0]);
 
	int texUniform = m_shader->getUniform("noiseTexture");
	glUniform1i(texUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlinTexture);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
