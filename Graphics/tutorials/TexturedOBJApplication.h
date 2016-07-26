#pragma once

#include "BaseApplication.h"

// forward declared to reduce dependencies
class Camera;
class Mesh;
class Shader;

class TexturedOBJApplication : public BaseApplication {
public:

	TexturedOBJApplication();
	virtual ~TexturedOBJApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*		m_camera;
	Mesh*		m_mesh;
	Shader*		m_shader;
};