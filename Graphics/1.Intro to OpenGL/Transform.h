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

	/// <summary>
	/// Rotates the transform matrix by 'angle' degrees on 'axis'
	/// </summary>
	/// <param name="angle">The angle in degrees to rotate</param>
	/// <param name="axis">The vector which defines the axis to rotate on</param>
	void rotate(const float& angle, const vec3& axis);

	/// <summary>
	/// Returns a modifiable pointer to the parent transform
	/// </summary>
	/// <returns>Modifiable pointer to the parent transform</returns>
	Transform* getParent();
	/// <summary>
	/// Sets the parent of the transform
	/// </summary>
	/// <param name="newParent">The new transform to parent to</param>
	/// <param name="keepWorldPosition">Whether or not to keep the transform's previous world position after</param>
	void setParent(Transform * newParent, const bool& keepWorldPosition = false);

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

	/// <summary>
	/// Parses the transform's world space matrix based on its parent transform and local space matrix
	/// </summary>
	/// <returns>The transform's world space matrix</returns>
	mat4 getWorldSpaceMatrix() const;
	/// <summary>
	/// Returns a non-modifiable reference to the 'm_matrix' variable
	/// </summary>
	/// <returns>Non-modifiable reference to the 'm_matrix' variable</returns>
	const mat4 & getLocalSpaceMatrix() const;

	~Transform();

private:
	
	// A pointer to the transform's parent transform
	Transform* m_parent = nullptr;

	// The transform's local space matrix
	mat4 m_matrix = mat4(1);

};

#endif // _TRANSFORM_H_
