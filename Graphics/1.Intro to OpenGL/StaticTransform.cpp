#include "Transform.h"

#include <imgui_impl_glfw_gl3.h>

#include "Math.h"
#include "Gizmos.h"

const GLfloat Transform::DEFAULT_LINE_WIDTH = 3.f;

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
void Transform::setPosition(mat4 *matrix, const vec3 &newPosition)
{
	(*matrix)[3] = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
}

mat4 Transform::eulerRotation(vec3 newEulerAngle)
{
	newEulerAngle = radians(newEulerAngle);

	// Create 3 identity matrices to hold each rotation on an axis
	auto rotationX = mat4(1);
	auto rotationY = mat4(1);
	auto rotationZ = mat4(1);

	// X Axis Rotation
	rotationX[1][1] = cos(newEulerAngle.x);
	rotationX[1][2] = sin(newEulerAngle.x);

	rotationX[2][1] = -sin(newEulerAngle.x);
	rotationX[2][2] = cos(newEulerAngle.x);

	// Y Axis Rotation
	rotationY[0][0] = cos(newEulerAngle.y);
	rotationY[0][2] = -sin(newEulerAngle.y);

	rotationY[2][0] = sin(newEulerAngle.y);
	rotationY[2][2] = cos(newEulerAngle.y);

	// Z Axis Rotation
	rotationZ[0][0] = cos(newEulerAngle.z);
	rotationZ[0][1] = sin(newEulerAngle.z);

	rotationZ[1][0] = -sin(newEulerAngle.z);
	rotationZ[1][1] = cos(newEulerAngle.z);

	// Apply
	return rotationZ * rotationY * rotationX;
}

vec3 Transform::getEulerAngle(const mat4 &matrix)
{
	// I wish I understood this, but I just don't right now
	// https://www.learnopencv.com/rotation-matrix-to-euler-angles/

	auto sy = sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1]);

	auto singular = sy < 1e-6;

	GLfloat x, y, z;
	if (!singular)
	{
		x = atan2(matrix[1][2], matrix[2][2]);
		y = atan2(-matrix[0][2], sy);
		z = atan2(matrix[0][1], matrix[0][0]);
	}
	else
	{
		x = atan2(-matrix[2][1], matrix[1][1]);
		y = atan2(-matrix[0][2], sy);
		z = 0;
	}

	auto eulerAngle = glm::degrees(vec3(x, y, z));
	eulerAngle = Math::clampAngle(eulerAngle);

	return eulerAngle;
}
void Transform::setEulerAngle(mat4 *matrix, const vec3 &newEulerAngle)
{
	// Grab the current rotation and create a new matrix to represent it
	auto oldEulerAngle = getEulerAngle(*matrix);
	auto oldRotationMatrix = eulerRotation(oldEulerAngle);
	// Remove the rotation from the current matrix
	*matrix *= inverse(oldRotationMatrix);

	// Apply the new rotation
	*matrix *= eulerRotation(newEulerAngle);
}

GLfloat Transform::getScale(const mat4 &matrix)
{
	// Grab the scale out of the matrix for each axis
	auto x = length(vec3(matrix[0].x, matrix[1].x, matrix[2].x));
	auto y = length(vec3(matrix[0].y, matrix[1].y, matrix[2].y));
	auto z = length(vec3(matrix[0].z, matrix[1].z, matrix[2].z));

	// Since we can't deal with non-uniform scaling, get the uniform scale by calculating the average
	return (x + y + z) / 3.f;
}
void Transform::setScale(mat4 * matrix, GLfloat newScale)
{
	// Get the quotient of the new scale and the current one
	auto deltaScale = newScale / getScale(*matrix);

	// Set the scale matrix based on 'deltaScale'
	auto scale =
		mat4(
			1.f * deltaScale, 0.f, 0.f, 0.f,
			0.f, 1.f * deltaScale, 0.f, 0.f,
			0.f, 0.f, 1.f * deltaScale, 0.f,
			0.f, 0.f, 0.f, 1.f);

	// Apply
	*matrix *= scale;
}

void Transform::drawGizmos(const mat4 &matrix, GLfloat lineWidth)
{
	auto endX = matrix * glm::translate(vec3(0.5f, 0.f, 0.f));
	auto endY = matrix * glm::translate(vec3(0.f, 0.5f, 0.f));
	auto endZ = matrix * glm::translate(vec3(0.f, 0.f, 0.5f));

	glDisable(GL_DEPTH_TEST);
	{
		Gizmos::drawLine(
			getPosition(matrix), vec3(endX[3].x, endX[3].y, endX[3].z), vec4(1.f, 0.f, 0.f, 1.f), lineWidth);
		Gizmos::drawLine(
			getPosition(matrix), vec3(endY[3].x, endY[3].y, endY[3].z), vec4(0.f, 1.f, 0.f, 1.f), lineWidth);
		Gizmos::drawLine(
			getPosition(matrix), vec3(endZ[3].x, endZ[3].y, endZ[3].z), vec4(0.f, 0.f, 1.f, 1.f), lineWidth);
	}
	glEnable(GL_DEPTH_TEST);
}

void Transform::drawGui(mat4 *matrix)
{
	static auto _eulerAngle = vec3();
	static void* _currentID = nullptr;

	if (!ImGui::IsAnyItemActive())
		_currentID = nullptr;

	auto position = getPosition(*matrix);
	auto eulerAngle = getEulerAngle(*matrix);
	auto scale = getScale(*matrix);

			if (ImGui::DragFloat3("Position", value_ptr(position), 0.01f))
				setPosition(matrix, position);

			if (ImGui::DragFloat3(
				"Rotation",
				_currentID == nullptr || _currentID != matrix ?
				value_ptr(eulerAngle) : value_ptr(_eulerAngle),
				0.1f))
			{
				if (_currentID != matrix)
				{
					_currentID = matrix;
					_eulerAngle = eulerAngle;
				}

				setEulerAngle(matrix, _eulerAngle);
			}

			if (ImGui::DragFloat("Scale", &scale, 0.01f, 0.01f, 50.f))
				setScale(matrix, scale);
}