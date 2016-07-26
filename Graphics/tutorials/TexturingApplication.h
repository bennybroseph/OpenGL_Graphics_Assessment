#pragma once

#include "BaseApplication.h"

// forward declared to reduce dependencies
class Camera;
class Shader;

class TexturingApplication : public BaseApplication {
public:

	TexturingApplication();
	virtual ~TexturingApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*		m_camera;
	Shader*		m_shader;

	unsigned int	m_vao, m_vbo, m_ibo;
	unsigned int	m_texture;
};