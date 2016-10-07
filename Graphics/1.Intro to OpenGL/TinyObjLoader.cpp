#include "TinyObjLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "Input.h"
#include "FlyCamera.h"
#include "Gizmos.h"

int TinyObjLoader::startup()
{
	createWindow("Tiny OBJ Loader", 1600, 900);

	Input::init();
	Shader::init();

	Gizmos::init();

	m_camera = make_unique<GameObject>();
	m_camera->setName("Main Camera");
	m_camera->addComponent<FlyCamera>();

	tinyobj::attrib_t attribs;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	string err;
	auto loaded = LoadObj(&attribs, &shapes, &materials, &err, "data/models/stanford/bunny.obj");

	createOpenGLBuffers(attribs, shapes);

	return true;
}

void TinyObjLoader::shutdown()
{
	Input::quit();
	Shader::quit();

	Gizmos::quit();

	destroyWindow();
}

void TinyObjLoader::parseInput()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// close the application if the window closes or we press escape
	if (glfwWindowShouldClose(m_window) || Input::getKey(GLFW_KEY_ESCAPE) >= GLFW_PRESS)
		m_isRunning = false;
}

void TinyObjLoader::update()
{
	m_camera->getComponent<FlyCamera>()->update(m_deltaTime);
}

void TinyObjLoader::lateUpdate()
{
	Input::lateUpdate();
}

void TinyObjLoader::draw()
{
	Gizmos::drawGrid(vec3(0.f), vec2(1.f, 1.f), vec2(5.f, 5.f));
	Transform::drawGizmos(mat4(1), 0);

	glUseProgram(Shader::basic()->programID());



	for (unsigned int i = 0; i < m_gl_info.size(); ++i)
	{
		glBindVertexArray(m_gl_info[i].m_VAO);
		for (int j = 0; j < 100; ++j)
		{
			unsigned int matUniform = glGetUniformLocation(Shader::basic()->programID(), "ProjectionViewModel");
			glUniformMatrix4fv(
				matUniform,
				1,
				false,
				value_ptr(Camera::mainCamera()->getProjectionView() * glm::translate(vec3(j, 0.f, 0.f))));
			glDrawElements(GL_TRIANGLES, m_gl_info[i].m_index_count, GL_UNSIGNED_INT, nullptr);
		}
	}
}

void TinyObjLoader::createOpenGLBuffers(tinyobj::attrib_t &attribs, vector<tinyobj::shape_t>& shapes)
{
	m_gl_info.resize(shapes.size());

	//for (unsigned int mesh_index = 0;
	//	mesh_index < shapes.size();
	//	++mesh_index)
	//{
	unsigned int mesh_index = 0;

	vector<GLuint> indexes;
	for (auto &index : shapes[mesh_index].mesh.indices)
	{
		indexes.push_back(index.vertex_index);
	}

	glGenVertexArrays(1, &m_gl_info[mesh_index].m_VAO);
	glGenBuffers(1, &m_gl_info[mesh_index].m_VBO);
	glGenBuffers(1, &m_gl_info[mesh_index].m_IBO);
	glBindVertexArray(m_gl_info[mesh_index].m_VAO);

	auto float_count = attribs.vertices.size();

	vector<float> vertex_data;
	vertex_data.reserve(float_count);
	vertex_data.insert(vertex_data.end(),
		attribs.vertices.begin(),
		attribs.vertices.end());
	vertex_data.insert(vertex_data.end(),
		attribs.normals.begin(),
		attribs.normals.end());

	m_gl_info[mesh_index].m_index_count =
		shapes[mesh_index].mesh.indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);
	glBufferData(GL_ARRAY_BUFFER,
		vertex_data.size() * sizeof(float),
		vertex_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexes.size() * sizeof(unsigned int),
		indexes.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //normal data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
		(void*)(sizeof(float) * attribs.vertices.size()));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}
}
