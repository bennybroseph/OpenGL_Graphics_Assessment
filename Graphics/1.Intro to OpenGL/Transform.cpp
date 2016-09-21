#include "Transform.h"

#include "Math.h"
#include "Gizmos.h"

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
void Transform::setParent(Transform *newParent, const bool &keepWorldTransformation)
{
	m_parent = newParent;

	// Makes sure the world space position, rotation and scale of the object before being parented is kept
	if (keepWorldTransformation)
	{
		setPosition(getLocalPosition());
		setEulerAngle(getLocalEulerAngle());
		setScale(getLocalScale());
	}
}

vec3 Transform::forward() const
{
	return forward(m_matrix);
}
vec3 Transform::back() const
{
	return back(getWorldSpaceMatrix());
}

vec3 Transform::right() const
{
	return right(getWorldSpaceMatrix());
}
vec3 Transform::left() const
{
	return left(getWorldSpaceMatrix());
}

vec3 Transform::up() const
{
	return up(getWorldSpaceMatrix());
}
vec3 Transform::down() const
{
	return down(getWorldSpaceMatrix());
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
		// Create a new Rotation matrix from the vec3 passed in
		auto newRotationMatrix = eulerRotation(newEulerAngle);

		// Grab the current rotation matrix
		auto oldRotationMatrix = eulerRotation(getLocalEulerAngle());
		// Remove the rotation from the current local space matrix
		m_matrix *= inverse(oldRotationMatrix);

		// Grab the rotation matrix out of the parent's world space matrix
		auto parentRotationMatrix = eulerRotation(getEulerAngle(m_parent->getWorldSpaceMatrix()));
		// Offset the local space matrix such that it's world space rotation is equal to 'newRotationMatrix'
		m_matrix *= inverse(parentRotationMatrix) * newRotationMatrix;
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

float Transform::getScale() const
{
	return getScale(getWorldSpaceMatrix());
}
void Transform::setScale(const float &newScale)
{
	if (m_parent != nullptr)
	{
		// Create a new Scale matrix from the vec3 passed in
		auto newScaleMatrix = mat4(1);
		setScale(newScaleMatrix, newScale);

		// Grab the current scale matrix
		auto oldScaleMatrix = mat4(1);
		setScale(oldScaleMatrix, getLocalScale());
		// Remove the scale from the current local space matrix
		m_matrix *= inverse(oldScaleMatrix);

		// Grab the scale matrix out of the parent's world space matrix
		auto parentScaleMatrix = mat4(1);
		setScale(parentScaleMatrix, getScale(m_parent->getWorldSpaceMatrix()));
		// Offset the local space matrix such that it's world space scale is equal to 'newScaleMatrix'
		m_matrix *= inverse(parentScaleMatrix) * newScaleMatrix;
	}
	else
		setLocalScale(newScale);
}

float Transform::getLocalScale() const
{
	return getScale(m_matrix);
}
void Transform::setLocalScale(const float &newScale)
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

void Transform::draw(const GLfloat& lineWidth) const
{
	draw(getWorldSpaceMatrix(), lineWidth);
}

Transform::~Transform() { }