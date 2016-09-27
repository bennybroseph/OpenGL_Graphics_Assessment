#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once

#include "Transform.h"

class Camera;

typedef unique_ptr<Camera> CameraPtrU;
typedef shared_ptr<Camera> CameraPtrS;
typedef weak_ptr<Camera> CameraPtrW;

class Camera
{
public:

	virtual void update(const float &deltaTime) const = 0;

	void setPerspective(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat newNear, GLfloat newFar) const;
	void setLookAt(const vec3 &from, const vec3 &to, const vec3 &up) const;
	void setPosition(const vec3 &position) const;

	const Transform& getWorldPosition() const;
	mat4 getView() const;
	const Transform& getProjection() const;

	mat4 getProjectionView() const;

	virtual ~Camera();

	static Camera *& mainCamera();

protected:

	Camera();

	static Camera *s_mainCamera;

	const TransformPtrU m_worldTransform = make_unique<Transform>(nullptr);
	const TransformPtrU m_projectionTransform = make_unique<Transform>(nullptr);

	const vec3PtrU m_rotation = make_unique<vec3>();

};

#endif // _CAMERA_H_