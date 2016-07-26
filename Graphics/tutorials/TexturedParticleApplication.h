// this example demonstrates texture particles with blending using custom emitter
#pragma once

#include "BaseApplication.h"
#include "Mesh.h"
#include "ParticleEmitter.h"

// forward declared to reduce dependencies
class Camera;
class Shader;

// emits on a disc in the XZ plane with particles moving up the Y axis
class CircleEmitter : public ParticleEmitter {
public:

	CircleEmitter() : m_radius(1) {}
	virtual ~CircleEmitter() {}

	void	setRadius(float radius) { m_radius = radius; }

protected:

	virtual void emit();

	float	m_radius;
};

class TexturedParticleApplication : public BaseApplication {
public:

	TexturedParticleApplication();
	virtual ~TexturedParticleApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*			m_camera;
	Shader*			m_shader;
	CircleEmitter*	m_emitter;
	AIETexture*		m_texture;
};