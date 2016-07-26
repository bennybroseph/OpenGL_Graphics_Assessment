#pragma once

#include "BaseApplication.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// forward declared to reduce dependencies
class Camera;
class Shader;

class LightingSphereApplication : public BaseApplication {
public:

	LightingSphereApplication();
	virtual ~LightingSphereApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	// generates sphere data and builds the vao, vbo, ibo
	void	generateSphere(unsigned int segments, unsigned int rings,
						   unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
						   unsigned int& indexCount);

	Camera*		m_camera;
	Shader*		m_shader;

	// define basic lighting
	struct DirectionalLight {
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} m_directionalLight;

	glm::vec3	m_ambientLight;

	// a generic material structure
	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float specularPower;
	} m_material;

	// opengl handles for the sphere
	unsigned int	m_vao, m_vbo, m_ibo, m_indexCount;

	// transform for the sphere mesh
	glm::mat4	m_modelMatrix;
};