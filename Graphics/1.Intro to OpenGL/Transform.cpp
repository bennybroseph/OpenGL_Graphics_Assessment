#include "Transform.h"
#include <glm/detail/type_mat.hpp>


Transform::Transform() { }

void Transform::translate(const vec3& translation)
{
	m_matrix *= glm::translate(translation);
}
void Transform::rotate(const float &angle, const vec3 &axis)
{
	m_matrix *= glm::rotate(glm::radians(angle), axis);
}
void Transform::scale(vec3 scale)
{
	m_matrix *= glm::scale(scale);
}

Transform& Transform::getParent() const
{
	return *m_parent;
}
void Transform::setParent(Transform *newParent, const bool &keepWorldPosition)
{
	m_parent = newParent;

	if (keepWorldPosition)
		setPosition(vec3(m_matrix[3].x, m_matrix[3].y, m_matrix[3].z));
}

vec3 Transform::getPosition() const
{
	return getPosition(getWorldSpaceMatrix());
}
void Transform::setPosition(const vec3 &newPosition)
{
	if (m_parent != nullptr)
	{
		// Create a new vector with a position of the parameter passed
		auto newWorldPosition = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
		// Set the vector such that the local position offsets the parent to equal the parameter passed
		newWorldPosition = inverse(m_parent->getWorldSpaceMatrix()) * newWorldPosition;

		// Update the local space matrix with the proper positional value
		setLocalPosition(vec3(newWorldPosition.x, newWorldPosition.y, newWorldPosition.z));
	}
	else
		setLocalPosition(newPosition);
}

vec3 Transform::getLocalPosition() const
{
	// Call the static function equivalent and give it this 'm_matrix'
	return getPosition(m_matrix);
}
void Transform::setLocalPosition(const vec3& newPosition)
{
	// Call the static function equivalent and give it this 'm_matrix'
	setPosition(m_matrix, newPosition);
}

vec3 Transform::getEulerAngle() const
{
	// Call the static function equivalent and give it this transform's matrix in world space
	return getEulerAngle(getWorldSpaceMatrix());
}
void Transform::setEulerAngle(const vec3 &newEulerAngle)
{
	if (m_parent != nullptr)
	{
		auto newMatrix = mat4(1);
		setEulerAngle(newMatrix, newEulerAngle);

		auto oldPosition = getLocalPosition();
		auto oldScale = getLocalScale();

		m_matrix = inverse(m_parent->getWorldSpaceMatrix()) * newMatrix;

		setLocalPosition(oldPosition);
		setLocalScale(oldScale);
	}
	else
		setLocalEulerAngle(newEulerAngle);
}

vec3 Transform::getLocalEulerAngle() const
{
	// Call the static function equivalent and give it this 'm_matrix'
	return getEulerAngle(m_matrix);
}
void Transform::setLocalEulerAngle(const vec3 &newEulerAngle)
{
	// Call the static function equivalent and give it this 'm_matrix'
	setEulerAngle(m_matrix, newEulerAngle);
}

vec3 Transform::getScale() const
{
	return getScale(getWorldSpaceMatrix());
}
void Transform::setScale(const vec3 &newScale)
{
	if (m_parent != nullptr)
	{
		auto newMatrix = mat4(1);
		setScale(newMatrix, newScale);

		auto oldScale = mat4(1);
		setScale(oldScale, getLocalScale());

		auto oldRotation = mat4(1);
		setEulerAngle(oldRotation, getLocalEulerAngle());

		auto oldPosition = mat4(1);
		setPosition(oldPosition, getLocalPosition());

		m_matrix = m_matrix * inverse(oldScale);
		//m_matrix = m_matrix * inverse(oldPosition);
		//m_matrix = m_matrix * inverse(oldRotation);
		m_matrix = inverse(m_parent->getWorldSpaceMatrix()) * newMatrix;

		//setLocalEulerAngle(oldRotation);
		//setLocalPosition(oldPosition);		
	}
	else
		setLocalScale(newScale);
}

vec3 Transform::getLocalScale() const
{
	return getScale(m_matrix);
}
void Transform::setLocalScale(const vec3 &newScale)
{
	setScale(m_matrix, newScale);
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

vec3 Transform::getPosition(const mat4 &matrix)
{
	return vec3(matrix[3].x, matrix[3].y, matrix[3].z);
}
void Transform::setPosition(mat4 &matrix, const vec3 &newPosition)
{
	matrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

vec3 Transform::getEulerAngle(const mat4 &matrix)
{
	auto x = glm::degrees(atan2(matrix[1].z, matrix[2].z));
	auto y = glm::degrees(atan2(-matrix[0].z, sqrt(pow(matrix[1].z, 2.f) + pow(matrix[2].z, 2.f))));
	auto z = glm::degrees(atan2(matrix[0].y, matrix[0].x));

	return vec3(x, y, z);
}
void Transform::setEulerAngle(mat4 &matrix, const vec3 &newEulerAngle)
{
	auto deltaAngle = newEulerAngle - getEulerAngle(matrix);
	deltaAngle = vec3(glm::radians(deltaAngle.x), glm::radians(deltaAngle.y), glm::radians(deltaAngle.z));

	auto rotationX = mat4(1);
	auto rotationY = mat4(1);
	auto rotationZ = mat4(1);

	rotationX[1].y = cos(deltaAngle.x);
	rotationX[1].z = sin(deltaAngle.x);

	rotationX[2].y = -sin(deltaAngle.x);
	rotationX[2].z = cos(deltaAngle.x);

	rotationY[0].x = cos(deltaAngle.y);
	rotationY[0].z = -sin(deltaAngle.y);

	rotationY[2].x = sin(deltaAngle.y);
	rotationY[2].z = cos(deltaAngle.y);

	rotationZ[0].x = cos(deltaAngle.z);
	rotationZ[0].y = sin(deltaAngle.z);

	rotationZ[1].x = -sin(deltaAngle.z);
	rotationZ[1].y = cos(deltaAngle.z);

	matrix *= rotationZ * rotationY * rotationX;
}

vec3 Transform::getScale(const mat4 &matrix)
{
	auto x = length(vec3(matrix[0].x, matrix[1].x, matrix[2].x));
	auto y = length(vec3(matrix[0].y, matrix[1].y, matrix[2].y));
	auto z = length(vec3(matrix[0].z, matrix[1].z, matrix[2].z));

	return vec3(x, y, z);
}
void Transform::setScale(mat4 &matrix, const vec3 &newScale)
{
	auto deltaScale = newScale / getScale(matrix);

	auto scale =
		mat4(
			vec4(deltaScale.x, 0.f, 0.f, 0.f),
			vec4(0.f, deltaScale.y, 0.f, 0.f),
			vec4(0.f, 0.f, deltaScale.z, 0.f),
			vec4(0.f, 0.f, 0.f, 1.f));

	matrix *= scale;
}
