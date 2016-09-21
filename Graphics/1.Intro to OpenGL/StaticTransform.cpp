#include "Transform.h"

#include "Math.h"
#include "Gizmos.h"

vec3 Transform::forward(const mat4 & matrix)
{
	return -back(matrix);
}
vec3 Transform::back(const mat4 & matrix)
{
	return normalize(vec3(matrix[2].x, matrix[2].y, matrix[2].z));
}

vec3 Transform::right(const mat4 & matrix)
{
	return normalize(vec3(matrix[0].x, matrix[0].y, matrix[0].z));
}
vec3 Transform::left(const mat4 & matrix)
{
	return -right(matrix);
}

vec3 Transform::up(const mat4 & matrix)
{
	return normalize(vec3(matrix[1].x, matrix[1].y, matrix[1].z));
}
vec3 Transform::down(const mat4 & matrix)
{
	return -up(matrix);
}

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

	auto eulerAngle = vec3(x, y, z);
	eulerAngle = Math::clampAngle(eulerAngle);

	return eulerAngle;
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

void Transform::draw(const mat4 &matrix, const GLfloat &lineWidth)
{
	auto f = right(matrix);
	f = up(matrix);
	f = back(matrix);

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