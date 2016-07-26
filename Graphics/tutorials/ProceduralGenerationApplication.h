#pragma once
#include "BaseApplication.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

// forward declared to reduce dependencies
class Camera;
class Shader;

class ProceduralGenerationApplication : public BaseApplication {
public:

	ProceduralGenerationApplication();
	virtual ~ProceduralGenerationApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*		m_camera;
	Shader*		m_shader;

	unsigned int	m_perlinDimensions;
	unsigned int	m_perlinTexture;
	float*			m_perlinData;

	// grid mesh data
	struct Vertex {
		glm::vec4 position;
		glm::vec2 texCoord;
	};
	unsigned int	m_vao, m_vbo, m_ibo;
	unsigned int	m_indexCount;
};