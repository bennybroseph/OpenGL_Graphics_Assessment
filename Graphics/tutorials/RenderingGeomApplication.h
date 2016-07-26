#pragma once

#include "BaseApplication.h"

#include <glm/vec4.hpp>

// forward declared to reduce dependencies
class Camera;

class RenderingGeomApplication : public BaseApplication {
public:

	RenderingGeomApplication();
	virtual ~RenderingGeomApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*		m_camera;

	struct Vertex {
		glm::vec4 position;
		glm::vec4 colour;
	};

	unsigned int	m_vao, m_vbo, m_ibo;
	unsigned int	m_indexCount;

	unsigned int	m_program;
};