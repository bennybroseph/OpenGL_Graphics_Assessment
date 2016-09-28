#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#pragma once

#include <gl_core_4_4.h>

#include "Component.h"

#include "MasterHeader.h"

class Transform;	// Forward declare class for typedef organization

typedef unique_ptr<Transform> TransformPtrU;
typedef shared_ptr<Transform> TransformPtrS;
typedef weak_ptr<Transform> TransformPtrW;

class Transform : public Component
{

	const static GLfloat DEFAULT_LINE_WIDTH;	// Used when drawing the transform axis lines

public:

	Transform() : Component() { }

	void translate(const vec3 &translation);
	/// <summary>
	/// Rotates the transform matrix by 'angle' degrees on 'axis'
	/// </summary>
	/// <param name="angle">The angle in degrees to rotate</param>
	/// <param name="axis">The vector which defines the axis to rotate on</param>
	void rotate(GLfloat angle, const vec3 &axis);
	void scale(const vec3 &scale);

	/// <summary>
	/// Returns a modifiable reference to the parent transform
	/// </summary>
	/// <returns>Modifiable reference to the parent transform</returns>
	Transform * getParent() const;
	/// <summary>
	/// Sets the parent of the transform
	/// </summary>
	/// <param name="newParent">The new transform to parent to</param>
	/// <param name="keepWorldPosition">Whether or not to keep the transform's previous world space position after</param>
	void setParent(Transform *newParent, GLboolean keepWorldPosition = true);

	vector<Transform *> * getChildren() const;

	vec3 forward() const;
	vec3 back() const;

	vec3 right() const;
	vec3 left() const;

	vec3 up() const;
	vec3 down() const;

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
	GLfloat getScale() const;
	/// <summary>
	/// Sets the transform's world space scale factor
	/// </summary>
	/// <param name="newPosition">The transform's new world space scale factor</param>
	void setScale(GLfloat newScale);

	/// <summary>
	/// Parses the local space scale factor out of a transform's local space matrix
	/// </summary>
	/// <returns>The transform's local space scale factor as a vec3</returns>
	GLfloat getLocalScale() const;
	/// <summary>
	/// Sets the transform's local space scale factor
	/// </summary>
	/// <param name="newPosition">The transform's new local space scale factor</param>
	void setLocalScale(GLfloat newScale);

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
	void setLocalSpaceMatrix(const mat4 &newLocalSpaceMatrix);

	void draw(GLfloat lineWidth = DEFAULT_LINE_WIDTH) const;
	void drawGui() const override;

	~Transform();

#pragma region // StaticTransform.cpp

	static vec3 forward(const mat4 &matrix);
	static vec3 back(const mat4 &matrix);

	static vec3 right(const mat4 &matrix);
	static vec3 left(const mat4 &matrix);

	static vec3 up(const mat4 &matrix);
	static vec3 down(const mat4 &matrix);

	static vec3 getPosition(const mat4 &matrix);
	static void setPosition(mat4 * matrix, const vec3 &newPosition);

	/// <summary>
	/// Creates a rotation matrix given Euler angles in degrees
	/// </summary>
	/// <param name="newEulerAngle">The new Euler angle rotation in degrees</param>
	/// <returns>A new rotation matrix given Euler angles in degrees</returns>
	static mat4 eulerRotation(vec3 newEulerAngle);

	static vec3 getEulerAngle(const mat4 &matrix);
	static void setEulerAngle(mat4 * matrix, const vec3 &newEulerAngle);

	static float getScale(const mat4 &matrix);
	static void setScale(mat4 * matrix, GLfloat newScale);

	static void draw(const mat4 &matrix, GLfloat lineWidth = DEFAULT_LINE_WIDTH);
	static void drawGui(mat4 * matrix);

#pragma endregion

private:

	// A pointer to the transform's immediate parent transform
	Transform *m_parent = nullptr;
	// A vector which contains all of the transform's children for caching purposes
	const vectorPtrU<Transform *> m_children = make_unique<vector<Transform *>>();

	// The transform's local space matrix
	const mat4PtrU m_matrix = make_unique<mat4>(mat4(1));

};

#endif // _TRANSFORM_H_
