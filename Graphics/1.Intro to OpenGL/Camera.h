#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Transform.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Camera
{
public:

	virtual void update(const float &deltaTime) const = 0;

	void setPerspective(
		const float &fieldOfView,
		const float &aspectRatio,
		const float &newNear,
		const float &newFar) const;
	void setLookAt(const vec3 &from, const vec3 &to, const vec3 &up) const;
	void setPosition(const vec3 &position) const;

	const Transform& getWorldPosition() const;
	mat4 getView() const;
	const Transform& getProjection() const;

	mat4 getProjectionView() const;

	virtual ~Camera();

	static Camera& mainCamera();

protected:

	static Camera *s_mainCamera;

	Transform *const m_worldTransform = new Transform();
	Transform *const m_projectionTransform = new Transform();

	vec3 *const m_rotation = new vec3();

};

#endif // _CAMERA_H_