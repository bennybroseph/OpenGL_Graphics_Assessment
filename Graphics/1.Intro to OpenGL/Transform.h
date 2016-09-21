#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm/ext.hpp>
#include <gl_core_4_4.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Transform
{
public:

	Transform();

	void translate(const vec3 &translation);
	/// <summary>
	/// Rotates the transform matrix by 'angle' degrees on 'axis'
	/// </summary>
	/// <param name="angle">The angle in degrees to rotate</param>
	/// <param name="axis">The vector which defines the axis to rotate on</param>
	void rotate(const float &angle, const vec3 &axis);
	void scale(vec3 scale);

	/// <summary>
	/// Returns a modifiable reference to the parent transform
	/// </summary>
	/// <returns>Modifiable reference to the parent transform</returns>
	Transform& getParent() const;
	/// <summary>
	/// Sets the parent of the transform
	/// </summary>
	/// <param name="newParent">The new transform to parent to</param>
	/// <param name="keepWorldPosition">Whether or not to keep the transform's previous world position after</param>
	void setParent(Transform *newParent, const bool &keepWorldPosition = true);

	/// <summary>
	/// Parses the world space position out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's world space position as a vec3</returns>
	vec3 getPosition() const;
	/// <summary>
	/// Sets the transform's world space position
	/// </summary>
	/// <param name="newPosition">The transform's new world space position</param>
	void setPosition(const vec3 &newPosition);

	/// <summary>
	/// Parses the local space position out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's local space position as a vec3</returns>
	vec3 getLocalPosition() const;
	/// <summary>
	/// Sets the transform's local space position
	/// </summary>
	/// <param name="newPosition">The transform's new local space position</param>
	void setLocalPosition(const vec3 &newPosition);

	/// <summary>
	/// Parses the world space rotation in degrees out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's world space rotation in degrees as a vec3</returns>
	vec3 getEulerAngle() const;
	/// <summary>
	/// Sets the transform's world space rotation using degrees
	/// </summary>
	/// <param name="newEulerAngle">The transform's new world space rotation in degrees</param>
	void setEulerAngle(const vec3 &newEulerAngle);

	/// <summary>
	/// Parses the local space rotation in degrees out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's local space rotation in degrees as a vec3</returns>
	vec3 getLocalEulerAngle() const;
	/// <summary>
	/// Sets the transform's local space rotation using degrees
	/// </summary>
	/// <param name="newEulerAngle">The transform's new local space rotation in degrees</param>
	void setLocalEulerAngle(const vec3 &newEulerAngle);

	/// <summary>
	/// Parses the world space scale factor out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's world space scale factor in degrees as a single float</returns>
	float getScale() const;
	/// <summary>
	/// Sets the transform's world space scale factor
	/// </summary>
	/// <param name="newPosition">The transform's new world space scale factor</param>
	void setScale(const float &newScale);

	/// <summary>
	/// Parses the local space scale factor out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's local space scale factor as a vec3</returns>
	float getLocalScale() const;
	/// <summary>
	/// Sets the transform's local space scale factor
	/// </summary>
	/// <param name="newPosition">The transform's new local space scale factor</param>
	void setLocalScale(const float &newScale);

	/// <summary>
	/// Parses the transform's world space matrix based on its parent transform and local space matrix
	/// </summary>
	/// <returns>The transform's world space matrix</returns>
	mat4 getWorldSpaceMatrix() const;

	/// <summary>
	/// Set's the transform's local space matrix
	/// </summary>
	/// <param name="newMatrix">The transform's new local space matrix</param>
	void setLocalSpaceMatrix(const mat4 &newMatrix);
	/// <summary>
	/// Returns a non-modifiable reference to the 'm_matrix' variable
	/// </summary>
	/// <returns>Non-modifiable reference to the 'm_matrix' variable</returns>
	const mat4 & getLocalSpaceMatrix() const;

	void draw(const GLfloat &lineWidth = 3.f) const;

	~Transform();

	static vec3 getPosition(const mat4 &matrix);
	static void setPosition(mat4 &matrix, const vec3 &newPosition);

	/// <summary>
	/// Creates a rotation matrix given Euler angles in degrees
	/// </summary>
	/// <param name="newEulerAngle">The new Euler angle rotation in degrees</param>
	/// <returns>A new rotation matrix given Euler angles in degrees</returns>
	static mat4 eulerRotation(vec3 newEulerAngle);

	static vec3 getEulerAngle(const mat4 &matrix);
	static void setEulerAngle(mat4 &matrix, const vec3 &newEulerAngle);

	static float getScale(const mat4 &matrix);
	static void setScale(mat4 &matrix, const float &newScale);

	static float clampAngle(float angle);
	static vec3 clampAngle(vec3 eulerAngle);

	static void draw(const mat4 &matrix, const GLfloat &lineWidth = 3.f);

private:

	// A pointer to the transform's parent transform
	Transform* m_parent = nullptr;

	// The transform's local space matrix
	mat4 m_matrix = mat4(1);

};

#endif // _TRANSFORM_H_
