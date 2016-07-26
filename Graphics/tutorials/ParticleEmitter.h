#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

// a simple particle structure with a few useful properties
struct Particle {
	glm::vec3	position;
	glm::vec3	velocity;
	glm::vec4	colour;
	float		size;

	float		lifetime;
	float		lifespan;
};

// a simple vertex for display purposes
struct ParticleVertex {
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec2 texcoord;
};

class ParticleEmitter {
public:

	ParticleEmitter();
	virtual ~ParticleEmitter();

	// set up the properties of the system and create buffers
	void	initialise(unsigned int maxParticles, unsigned int emitRate,
					   float lifespanMin, float lifespanMax,
					   float velocityMin, float velocityMax,
					   float startSize, float endSize,
					   const glm::vec4& startColour, const glm::vec4& endColour);

	// update the living particles, emit new particles, and update billboarded vertex buffer
	void	update(float deltaTime, const glm::mat4& cameraTransform);

	// repopulates vertex buffer and draws living particles
	void	draw();

	// not in the tutorial but useful
	inline void	setTransform(const glm::mat4& transform)	{ m_transform = transform; }
	inline const glm::mat4& getTransform() const			{ return m_transform; }

protected:

	// emitsa particle out of a point with a random direction
	virtual void	emit();

	// transform that the particles are spawned out of
	glm::mat4		m_transform;

	// render data
	// we keep the array so that we can repopulate it
	ParticleVertex*	m_vertices;
	unsigned int	m_vao, m_vbo, m_ibo;

	// the particles themselves
	Particle*		m_particles;
	unsigned int	m_aliveParticles;
	unsigned int	m_maxParticles;

	// tracks how much time should elapse before a particle emits
	float			m_emitTimer;
	float			m_emitRate;

	// properties for the particles
	float			m_lifespanMin;
	float			m_lifespanMax;

	float			m_velocityMin;
	float			m_velocityMax;

	float			m_startSize;
	float			m_endSize;

	glm::vec4		m_startColour;
	glm::vec4		m_endColour;
};