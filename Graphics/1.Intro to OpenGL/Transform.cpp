#include "Transform.h"

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

vec3 Transform::getPosition(const mat4 &matrix)
{
	return vec3(matrix[3].x, matrix[3].y, matrix[3].z);
}
void Transform::setPosition(mat4 &matrix, const vec3 &newPosition)
{
	matrix[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

mat4 Transform::eulerRotation(vec3 newEulerAngle)
{
	newEulerAngle = radians(newEulerAngle);

	// Create 3 identity matrices to hold each rotation on an axis
	auto rotationX = mat4(1);
	auto rotationY = mat4(1);
	auto rotationZ = mat4(1);

	// X Axis Rotation
	rotationX[1].y = cos(newEulerAngle.x);
	rotationX[1].z = sin(newEulerAngle.x);

	rotationX[2].y = -sin(newEulerAngle.x);
	rotationX[2].z = cos(newEulerAngle.x);

	// Y Axis Rotation
	rotationY[0].x = cos(newEulerAngle.y);
	rotationY[0].z = -sin(newEulerAngle.y);

	rotationY[2].x = sin(newEulerAngle.y);
	rotationY[2].z = cos(newEulerAngle.y);

	// Z Axis Rotation
	rotationZ[0].x = cos(newEulerAngle.z);
	rotationZ[0].y = sin(newEulerAngle.z);

	rotationZ[1].x = -sin(newEulerAngle.z);
	rotationZ[1].y = cos(newEulerAngle.z);

	// Apply
	return rotationZ * rotationY * rotationX;
}

vec3 Transform::getEulerAngle(const mat4 &matrix)
{
	// Grab the angle of rotation in degrees from the matrix on each axis
	auto x = glm::degrees(atan2(matrix[1].z, matrix[2].z));
	auto y = glm::degrees(atan2(-matrix[0].z, sqrt(pow(matrix[1].z, 2.f) + pow(matrix[2].z, 2.f))));
	auto z = glm::degrees(atan2(matrix[0].y, matrix[0].x));

	x = clampAngle(x);
	y = clampAngle(y);
	z = clampAngle(z);

	return vec3(x, y, z);
}
void Transform::setEulerAngle(mat4 &matrix, const vec3 &newEulerAngle)
{
	// Grab the current rotation and create a new matrix to represent it
	auto oldEulerAngle = getEulerAngle(matrix);
	auto oldRotationMatrix = eulerRotation(oldEulerAngle);
	// Remove the rotation from the current matrix
	matrix *= inverse(oldRotationMatrix);

	// Apply the new rotation
	matrix *= eulerRotation(newEulerAngle);
}

float Transform::getScale(const mat4 &matrix)
{
	// Grab the scale out of the matrix for each axis
	auto x = length(vec3(matrix[0].x, matrix[1].x, matrix[2].x));
	auto y = length(vec3(matrix[0].y, matrix[1].y, matrix[2].y));
	auto z = length(vec3(matrix[0].z, matrix[1].z, matrix[2].z));

	// Since we can't deal with non-uniform scaling, get the uniform scale by calculating the average
	return (x + y + z) / 3.f;
}
void Transform::setScale(mat4 &matrix, const float &newScale)
{
	// Get the quotient of the new scale and the current one
	auto deltaScale = newScale / getScale(matrix);

	// Set the scale matrix based on 'deltaScale'
	auto scale =
		mat4(
			1.f * deltaScale, 0.f, 0.f, 0.f,
			0.f, 1.f * deltaScale, 0.f, 0.f,
			0.f, 0.f, 1.f * deltaScale, 0.f,
			0.f, 0.f, 0.f, 1.f);

	// Apply
	matrix *= scale;
}

float Transform::clampAngle(float angle)
{
	while (angle >= 360.f)
		angle -= 360.f;
	while (angle < 0.f)
		angle += 360.f;

	angle = abs(angle);

	return angle;
}
vec3 Transform::clampAngle(vec3 eulerAngle)
{
	clampAngle(eulerAngle.x);
	clampAngle(eulerAngle.y);
	clampAngle(eulerAngle.z);

	return eulerAngle;
}

void Transform::draw(const mat4 &matrix, const GLfloat &lineWidth)
{
	auto endX = matrix * glm::translate(vec3(0.5f, 0.f, 0.f));
	auto endY = matrix * glm::translate(vec3(0.f, 0.5f, 0.f));
	auto endZ = matrix * glm::translate(vec3(0.f, 0.f, 0.5f));

	glDisable(GL_DEPTH_TEST);
	{
		Gizmos::drawLine(getPosition(matrix), vec3(endX[3].x, endX[3].y, endX[3].z), vec4(1.f, 0.f, 0.f, 1.f), lineWidth);
		Gizmos::drawLine(getPosition(matrix), vec3(endY[3].x, endY[3].y, endY[3].z), vec4(0.f, 1.f, 0.f, 1.f), lineWidth);
		Gizmos::drawLine(getPosition(matrix), vec3(endZ[3].x, endZ[3].y, endZ[3].z), vec4(0.f, 0.f, 1.f, 1.f), lineWidth);
	}
	glEnable(GL_DEPTH_TEST);
}
