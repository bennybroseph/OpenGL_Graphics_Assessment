#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Transform
{
public:

	Transform();

	void update();

	void rotate(const float& angle, const vec3& axis);

	void setParent(const Transform * newParent);

	vec3 getPosition();
	void setPosition(const vec3 &newPosition);

	vec3 getLocalPosition();
	void setLocalPosition(const vec3 &newPosition);

	const mat4& getMatrix() const;

	~Transform();

private:

	const Transform* m_parent = nullptr;

	mat4 m_localMatrix = mat4(1);
	mat4 m_worldMatrix = mat4(1);

};

#endif // _TRANSFORM_H_
