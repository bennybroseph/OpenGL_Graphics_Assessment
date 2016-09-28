#include "Camera.h"

#include "GameObject.h"

Camera *Camera::s_mainCamera = nullptr;

Camera::Camera() : Component()
{
	if (s_mainCamera == nullptr)
		s_mainCamera = this;

	setName("Camera");
}

void Camera::setPerspective(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat newNear, GLfloat newFar) const
{
	*m_projectionTransform = glm::perspective(fieldOfView, aspectRatio, newNear, newFar);
}
void Camera::setLookAt(const vec3 &from, const vec3 &to, const vec3 &up) const
{
	auto zAxis = normalize(from - to);
	auto xAxis = normalize(cross(up, zAxis));
	auto yAxis = cross(zAxis, xAxis);

	auto orientation =
		mat4(
			xAxis.x, yAxis.x, zAxis.x, 0,
			xAxis.y, yAxis.y, zAxis.y, 0,
			xAxis.z, yAxis.z, zAxis.z, 0,
			0, 0, 0, 1);

	auto translation =
		mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-from.x, -from.y, -from.z, 1);

	m_gameObject->transform()->setLocalSpaceMatrix(inverse(orientation * translation));
	*m_rotation = m_gameObject->transform()->getEulerAngle();
}
void Camera::setPosition(const vec3& position) const
{
	m_gameObject->transform()->setLocalPosition(position);
}

const Transform& Camera::getWorldPosition() const
{
	return *m_gameObject->transform();
}
mat4 Camera::getView() const
{
	return inverse(m_gameObject->transform()->getLocalSpaceMatrix());
}
const mat4 & Camera::getProjection() const
{
	return *m_projectionTransform;
}

mat4 Camera::getProjectionView() const
{
	return *m_projectionTransform * getView();
}

Camera::~Camera()
{
	if (s_mainCamera == this)
		s_mainCamera = nullptr;
}

Camera *& Camera::mainCamera()
{
	return s_mainCamera;
}