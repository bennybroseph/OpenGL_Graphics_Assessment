#include "Camera.h"



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
		xAxis[0], yAxis[0], zAxis[0], 0,
		xAxis[1], yAxis[1], zAxis[1], 0,
		xAxis[2], yAxis[2], zAxis[2], 0,
		0, 0, 0, 1);

	auto translation = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-from[0], -from[1], -from[2], 1);

	m_worldTransform.setLocalSpaceMatrix(inverse(orientation * translation));
}
void Camera::setPosition(const vec3& position)
{
	m_worldTransform.setLocalPosition(position);
}

const Transform& Camera::getWorldPosition() const
{
	return m_worldTransform;
}
const Transform& Camera::getView() const
{
	return m_viewTransform;
}
const Transform& Camera::getProjection() const
{
	return m_projectionTransform;
}

mat4 Camera::getProjectionView() const
{
	return
		m_projectionTransform.getLocalSpaceMatrix() *
		inverse(m_worldTransform.getLocalSpaceMatrix());
}


void Camera::updateProjectionViewTransform()
{

}

Camera::~Camera() { }