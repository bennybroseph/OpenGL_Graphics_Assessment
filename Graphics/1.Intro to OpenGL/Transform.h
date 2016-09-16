#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm/ext.hpp>
#include <glm/detail/_noise.hpp>
#include <glm/detail/type_mat.hpp>
//#include <glm/detail/type_mat.hpp>

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
	void setParent(Transform *newParent, const bool &keepWorldPosition = false);

	/// <summary>
	/// Parses the world position out of a transform's world space matrix
	/// </summary>
	/// <returns>The transform's world position as a vec3</returns>
	vec3 getPosition() const;
	/// <summary>
	/// Set's the transform's world position
	/// </summary>
	/// <param name="newPosition">The transform's new world position</param>
	void setPosition(const vec3 &newPosition);

	/// <summary>
	/// Parses the local position out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's local position as a vec3</returns>
	vec3 getLocalPosition() const;
	/// <summary>
	/// Set's the transform's local position
	/// </summary>
	/// <param name="newPosition">The transform's new local position</param>
	void setLocalPosition(const vec3 &newPosition);

	vec3 getEulerAngle() const;
	void setEulerAngle(const vec3 &newEulerAngle);

	vec3 getLocalEulerAngle() const;
	void setLocalEulerAngle(const vec3 &newEulerAngle);

	vec3 getScale() const;
	void setScale(const vec3 &newScale);

	vec3 getLocalScale() const;
	void setLocalScale(const vec3 &newScale);

	/// <summary>
	/// Parses the transform's world space matrix based on its parent transform and local space matrix
	/// </summary>
	/// <returns>The transform's world space matrix</returns>
	mat4 getWorldSpaceMatrix() const;

	void setLocalSpaceMatrix(const mat4 &newMatrix);
	/// <summary>
	/// Returns a non-modifiable reference to the 'm_matrix' variable
	/// </summary>
	/// <returns>Non-modifiable reference to the 'm_matrix' variable</returns>
	const mat4 & getLocalSpaceMatrix() const;

	~Transform();

	static vec3 getPosition(const mat4 &matrix);
	static void setPosition(mat4 &matrix, const vec3 &newPosition);

	static vec3 getEulerAngle(const mat4 &matrix);
	static void setEulerAngle(mat4 &matrix, const vec3 &newEulerAngle);

	static vec3 getScale(const mat4 &matrix);
	static void setScale(mat4 &matrix, const vec3 &newScale);

private:

	// A pointer to the transform's parent transform
	Transform* m_parent = nullptr;

	// The transform's local space matrix
	mat4 m_matrix = mat4(1);

};

#endif // _TRANSFORM_H_
