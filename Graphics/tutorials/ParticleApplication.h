#pragma once

#include "BaseApplication.h"

// forward declared to reduce dependencies
class Camera;
class Shader;
class ParticleEmitter;

class ParticleApplication : public BaseApplication {
public:

	ParticleApplication();
	virtual ~ParticleApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*				m_camera;
	Shader*				m_shader;
	ParticleEmitter*	m_emitter;
};