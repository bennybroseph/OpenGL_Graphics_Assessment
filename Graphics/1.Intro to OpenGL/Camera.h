#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/ext.hpp>

#include "Transform.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:

	virtual void update(const float &deltaTime) = 0;

	void setPerspective(
		const float &fieldOfView, const float &aspectRatio, const float &newNear, const float &newFar);
	void setLookAt(const vec3 &from, const vec3 &to, const vec3 &up);
	void setPosition(const vec3 &position);

	const Transform& getWorldPosition() const;
	const Transform& getView() const;
	const Transform& getProjection() const;

	mat4 getProjectionView() const;

	virtual ~Camera();

protected:

	void updateProjectionViewTransform();

	Transform m_worldTransform;
	Transform m_viewTransform;
	Transform m_projectionTransform;

};

#endif // _CAMERA_H_