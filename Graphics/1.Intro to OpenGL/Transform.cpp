#include "Transform.h"



Transform::Transform() { }

void Transform::update()
{
	if (m_parent != nullptr)
		m_worldMatrix = m_parent->getMatrix() * m_localMatrix;
	else
		m_worldMatrix = m_localMatrix;
}

void Transform::rotate(const float& angle, const vec3& axis)
{
	m_localMatrix = glm::rotate(m_localMatrix, angle, axis);
}

void Transform::setParent(const Transform * newParent)
{
	m_parent = newParent;
}

vec3 Transform::getPosition()
{
	return vec3(m_worldMatrix[3].x, m_worldMatrix[3].y, m_worldMatrix[3].z);
}

void Transform::setPosition(const vec3 &newPosition)
{
	if (m_parent != nullptr)
		m_localMatrix[3] = vec4(
			newPosition.x - m_parent->getMatrix()[3].x,
			newPosition.y - m_parent->getMatrix()[3].y,
			newPosition.z - m_parent->getMatrix()[3].z, 1);
	else
		m_localMatrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

vec3 Transform::getLocalPosition()
{
	return vec3(m_localMatrix[3].x, m_localMatrix[3].y, m_localMatrix[3].z);
}

void Transform::setLocalPosition(const vec3& newPosition)
{
	m_localMatrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

const mat4& Transform::getMatrix() const
{
	return m_worldMatrix;
}

Transform::~Transform() { }
