#include "ParticleEmitter.h"
#include "gl_core_4_4.h"
#include <glm/gtc/random.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

ParticleEmitter::ParticleEmitter()
	: m_particles(nullptr),
	m_aliveParticles(0),
	m_maxParticles(0),
	m_transform(1),
	m_vao(0), m_vbo(0), m_ibo(0),
	m_vertices(nullptr) {
}

ParticleEmitter::~ParticleEmitter() {
	delete[] m_particles;
	delete[] m_vertices;
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::initialise(unsigned int maxParticles,
								 unsigned int emitRate,
								 float lifetimeMin, float lifetimeMax,
								 float velocityMin, float velocityMax,
								 float startSize, float endSize,
								 const glm::vec4& startColour, const glm::vec4& endColour) {
	// set up emit timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / emitRate;

	// store all variables passed in
	m_startColour = startColour;
	m_endColour = endColour;

	m_startSize = startSize;
	m_endSize = endSize;

	m_velocityMin = velocityMin;
	m_velocityMax = velocityMax;

	m_lifespanMin = lifetimeMin;
	m_lifespanMax = lifetimeMax;
	
	m_maxParticles = maxParticles;

	// create particle array
	m_particles = new Particle[m_maxParticles];

	// none are alive yet
	m_aliveParticles = 0;

	// create the array of vertices for the particles
	// 4 vertices per particle for a quad
	// will be filled during update
	m_vertices = new ParticleVertex[m_maxParticles * 4];

	// create the index buffer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; ++i) {
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// create opengl buffers
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	// create the vertex array object and populate it
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 *
				 sizeof(ParticleVertex), m_vertices,
				 GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 *
				 sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glEnableVertexAttribArray(2); // texcoord
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)sizeof(glm::vec4));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(sizeof(glm::vec4)*2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void ParticleEmitter::emit() {

	// don't emit if we have no dead particles to ressurrect
	if (m_aliveParticles >= m_maxParticles)
		return;

	// access the first dead particle and increase alive count
	Particle& particle = m_particles[m_aliveParticles++];

	// start its position at the position of the emitter
	particle.position = vec3(m_transform[3]);

	// give it a random direction and set velocity
	float v = glm::linearRand(m_velocityMin, m_velocityMax);
	particle.velocity = glm::sphericalRand(v);

	// randomise its lifespan
	particle.lifetime = 0;
	particle.lifespan = glm::linearRand(m_lifespanMin, m_lifespanMax);

	// give it starting properties
	particle.colour = m_startColour;
	particle.size = m_startSize;
}

void ParticleEmitter::draw() {

	// update the opengl buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER,
					0, m_aliveParticles * 4 * sizeof(ParticleVertex),
					m_vertices);

	// draw the alive particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_aliveParticles * 6, GL_UNSIGNED_INT, 0);
}

void ParticleEmitter::update(float deltaTime, const glm::mat4& cameraTransform) {

	// emit particles
	// we loop and use a timer so that we can correctly spawn
	// the right amount
	// if we didn't and spawned one per frame, or didn't count back
	// the timer then we have an artificial emit rate of 60 per second
	m_emitTimer += deltaTime;
	while (m_emitTimer > m_emitRate) {
		emit();
		m_emitTimer -= m_emitRate;
	}

	// update living particles (we don't increment 'i' if it is dead!)
	for (unsigned int i = 0; i < m_aliveParticles; ) {

		// access it with a pointer because of how we "kill" particles
		Particle* particle = &m_particles[i];

		// update life
		particle->lifetime += deltaTime;

		// is it dead?
		if (particle->lifetime >= particle->lifespan) {
			// if dead we swap it with the last living particle then
			// reduce the living count
			*particle = m_particles[--m_aliveParticles];
		}
		else {

			// if alive we update the position and interpolate the size / colour
			particle->position += particle->velocity * deltaTime;

			particle->size = glm::mix(m_startSize, m_endSize,
									  particle->lifetime / particle->lifespan);

			particle->colour = glm::mix(m_startColour, m_endColour,
										particle->lifetime / particle->lifespan);

			// create billboard transform so it can face the camera
			vec3 zAxis = glm::normalize(vec3(cameraTransform[3]) - particle->position);
			vec3 xAxis = glm::cross(vec3(cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(vec4(xAxis, 0),
								vec4(yAxis, 0),
								vec4(zAxis, 0),
								vec4(0, 0, 0, 1));

			float halfSize = particle->size * 0.5f;

			m_vertices[i * 4 + 0].position = billboard * vec4(halfSize, halfSize, 0, 1) + vec4(particle->position,0);
			m_vertices[i * 4 + 1].position = billboard * vec4(-halfSize, halfSize, 0, 1) + vec4(particle->position,0);
			m_vertices[i * 4 + 2].position = billboard * vec4(-halfSize, -halfSize, 0, 1) + vec4(particle->position,0);
			m_vertices[i * 4 + 3].position = billboard * vec4(halfSize, -halfSize, 0, 1) + vec4(particle->position,0);

			m_vertices[i * 4 + 0].colour = particle->colour;
			m_vertices[i * 4 + 1].colour = particle->colour;
			m_vertices[i * 4 + 2].colour = particle->colour;
			m_vertices[i * 4 + 3].colour = particle->colour;

			// NOTE: this isn't in the tutorial but gives us the ability to texture particles
			m_vertices[i * 4 + 0].texcoord = glm::vec2(1,1);
			m_vertices[i * 4 + 1].texcoord = glm::vec2(0,1);
			m_vertices[i * 4 + 2].texcoord = glm::vec2(0,0);
			m_vertices[i * 4 + 3].texcoord = glm::vec2(1,0);

			++i;
		}
	}
}