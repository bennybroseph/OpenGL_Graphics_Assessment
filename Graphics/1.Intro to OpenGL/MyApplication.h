#pragma once
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "BaseApplication.h"

class MyApplication : public BaseApplication {
public:
	virtual bool startup();

private:

	// Inherited via BaseApplication
	virtual void shutdown() override;
	virtual bool update(float deltaTime) override;
	virtual void draw() override;
	glm::mat4 m_view;
	glm::mat4 m_projection;
};