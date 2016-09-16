#include "Camera.h"

Camera* Camera::s_mainCamera;

void Camera::setPerspective(const float &fieldOfView, const float& aspectRatio, const float &newNear, const float &newFar)
{
	m_projectionTransform.setLocalSpaceMatrix(
		glm::perspective(fieldOfView, aspectRatio, newNear, newFar));
}
void Camera::setLookAt(const vec3 &from, const vec3 &to, const vec3 &up)
{
	auto zAxis = normalize(from - to);
	auto xAxis = normalize(cross(up, zAxis));
	auto yAxis = cross(zAxis, xAxis);

	auto orientation = mat4(
		xAxis.x, yAxis.x, zAxis.x, 0,
		xAxis.y, yAxis.y, zAxis.y, 0,
		xAxis.z, yAxis.z, zAxis.z, 0,
		0, 0, 0, 1);

	auto translation = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-from.x, -from.y, -from.z, 1);

	m_worldTransform.setLocalSpaceMatrix(inverse(orientation * translation));
}
void Camera::setPosition(const vec3& position)
{
	m_worldTransform.setLocalPosition(position);
}

Camera& Camera::mainCamera()
{
	return *s_mainCamera;
}

const Transform& Camera::getWorldPosition() const
{
	return m_worldTransform;
}
mat4 Camera::getView() const
{
	return inverse(m_worldTransform.getLocalSpaceMatrix());
}
const Transform& Camera::getProjection() const
{
	return m_projectionTransform;
}

mat4 Camera::getProjectionView() const
{
	return
		m_projectionTransform.getLocalSpaceMatrix() * getView();
}

Camera::~Camera() { }