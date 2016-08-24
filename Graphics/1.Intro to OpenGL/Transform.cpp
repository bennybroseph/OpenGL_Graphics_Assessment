#include "Transform.h"

Transform::Transform() { }

void Transform::translate(const vec3& translation)
{
	m_matrix *= glm::translate(translation);
}
void Transform::rotate(const float &angle, const vec3 &axis)
{
	m_matrix *= glm::rotate(glm::radians(angle), axis);
}

Transform* Transform::getParent()
{
	return m_parent;
}
void Transform::setParent(Transform *newParent, const bool &keepWorldPosition)
{
	m_parent = newParent;

	if (keepWorldPosition)
		setPosition(vec3(m_matrix[3].x, m_matrix[3].y, m_matrix[3].z));
}

vec3 Transform::getPosition() const
{
	if (m_parent != nullptr)
	{
		auto worldMatrix = getWorldSpaceMatrix();
		return vec3(worldMatrix[3].x, worldMatrix[3].y, worldMatrix[3].z);
	}

	return getLocalPosition();
}
void Transform::setPosition(const vec3 &newPosition)
{
	if (m_parent != nullptr)
	{
		// Create a new matrix with a position of the parameter passed
		auto newWorldSpaceMatrix = mat4(1);
		newWorldSpaceMatrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
		// Set the matrix such that the local position offsets the parent to equal the parameter passed
		newWorldSpaceMatrix = inverse(m_parent->getWorldSpaceMatrix()) * newWorldSpaceMatrix;

		// Update the local space matrix with the proper positional value
		m_matrix[3] = vec4(
			newWorldSpaceMatrix[3].x,
			newWorldSpaceMatrix[3].y,
			newWorldSpaceMatrix[3].z, 1);
	}
	else
		m_matrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

vec3 Transform::getLocalPosition() const
{
	return vec3(m_matrix[3].x, m_matrix[3].y, m_matrix[3].z);
}
void Transform::setLocalPosition(const vec3& newPosition)
{
	m_matrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

mat4 Transform::getWorldSpaceMatrix() const
{
	if (m_parent != nullptr)
		return m_parent->getWorldSpaceMatrix() * m_matrix;

	return getLocalSpaceMatrix();
}

void Transform::setLocalSpaceMatrix(const mat4 &newMatrix)
{
	m_matrix = newMatrix;
}

const mat4 & Transform::getLocalSpaceMatrix() const
{
	return m_matrix;
}

Transform::~Transform() { }
