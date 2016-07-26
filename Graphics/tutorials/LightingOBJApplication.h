#pragma once

#include "BaseApplication.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// forward declared to reduce dependencies
class Camera;
class Shader;
class Mesh;

class LightingOBJApplication : public BaseApplication {
public:

	LightingOBJApplication();
	virtual ~LightingOBJApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	// define basic lighting
	struct DirectionalLight {
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} m_directionalLight;

	glm::vec3	m_ambientLight;

	Camera*		m_camera;
	Shader*		m_shader;
	Mesh*		m_mesh;

	// transform for the loaded mesh
	glm::mat4	m_modelMatrix;
};