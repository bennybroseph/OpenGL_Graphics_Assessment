#include "Transform.h"
#include <stack>


Transform::Transform() : Component()
{
	setName("Transform");
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::translate(const vec3& translation)
{
	*m_matrix *= glm::translate(translation);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::rotate(GLfloat angle, const vec3 &axis)
{
	*m_matrix *= glm::rotate(glm::radians(angle), axis);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::scale(const vec3 &scale)
{
	*m_matrix *= glm::scale(scale);
}

Transform * Transform::getParent() const
{
	return m_parent;
}

void Transform::grabChildrenInOrder(void(*delegate)(Transform *))
{
	auto transforms = std::stack<Transform *>();
	transforms.push(this);

	auto currentTransform = transforms.top();
	auto foundOne = false;
	while (transforms.size() > 0)
	{
		for (auto &child : *currentTransform->m_children)
		{
			delegate(child);

			transforms.push(child);
		}

		currentTransform = transforms.top();
		transforms.pop();
	}
}
void Transform::setParent(Transform * newParent, GLboolean keepWorldTransformation)
{
	if (m_parent == newParent || m_parent == this)
		return;

	if (m_parent)
	{
		auto i = 0;
		for (auto &child : *m_parent->m_children)
		{
			if (child->getID() == getID())
			{
				m_parent->m_children->erase(m_parent->m_children->begin() + i);
				break;
			}

			i++;
		}
	}

	auto parentToChild = false;
	if (m_children->size() > 0)
	{
		auto transforms = std::stack<Transform *>();
		transforms.push(this);

		auto currentTransform = transforms.top();
		while (transforms.size() > 0)
		{
			for (auto &child : *currentTransform->m_children)
			{
				if (child == newParent)
				{
					parentToChild = true;
					break;
				}

				transforms.push(child);
			}
			if (parentToChild)
				break;

			currentTransform = transforms.top();
			transforms.pop();
		}
	}

	if (parentToChild)
	{
		auto tempVector = *m_children;
		for (auto &child : tempVector)
			child->setParent(m_parent);

		m_children->clear();
	}

	vec3 oldPosition;
	vec3 oldEulerAngle;
	GLfloat oldScale = 1.f;
	if (keepWorldTransformation)
	{
		oldPosition = getPosition();
		oldEulerAngle = getEulerAngle();
		oldScale = getScale();
	}

	m_parent = newParent;

	if (m_parent)
		m_parent->m_children->push_back(this);

	// Makes sure the world space position, rotation and scale of the object before being parented is kept
	if (keepWorldTransformation)
	{
		setPosition(oldPosition);
		setEulerAngle(oldEulerAngle);
		setScale(oldScale);
	}
}

vector<Transform*>* Transform::getChildren() const
{
	return m_children.get();
}

vec3 Transform::forward() const
{
	return forward(getWorldSpaceMatrix());
}
vec3 Transform::back() const
{
	return back(getWorldSpaceMatrix());
}

vec3 Transform::right() const
{
	return right(getWorldSpaceMatrix());
}
vec3 Transform::left() const
{
	return left(getWorldSpaceMatrix());
}

vec3 Transform::up() const
{
	return up(getWorldSpaceMatrix());
}
vec3 Transform::down() const
{
	return down(getWorldSpaceMatrix());
}

vec3 Transform::getPosition() const
{
	return getPosition(getWorldSpaceMatrix());
}
void Transform::setPosition(const vec3 &newPosition)
{
	if (m_parent != nullptr)
	{
		// Create a new vector with a position of the parameter passed
		auto newWorldPosition = vec4(newPosition.x, newPosition.y, newPosition.z, 1);
		// Set the vector such that the local position offsets the parent to equal the parameter passed
		newWorldPosition = inverse(m_parent->getWorldSpaceMatrix()) * newWorldPosition;

		// Update the local space matrix with the proper positional value
		setLocalPosition(vec3(newWorldPosition.x, newWorldPosition.y, newWorldPosition.z));
	}
	else
		setLocalPosition(newPosition);
}

vec3 Transform::getLocalPosition() const
{
	// Call the static function equivalent and give it this 'm_matrix'
	return getPosition(*m_matrix);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::setLocalPosition(const vec3& newPosition)
{
	// Call the static function equivalent and give it this 'm_matrix'
	setPosition(m_matrix.get(), newPosition);
}

vec3 Transform::getEulerAngle() const
{
	// Call the static function equivalent and give it this transform's matrix in world space
	return getEulerAngle(getWorldSpaceMatrix());
}
void Transform::setEulerAngle(const vec3 &newEulerAngle)
{
	if (m_parent != nullptr)
	{
		// Create a new Rotation matrix from the vec3 passed in
		auto newRotationMatrix = eulerRotation(newEulerAngle);

		// Grab the current rotation matrix
		auto oldRotationMatrix = eulerRotation(getLocalEulerAngle());
		// Remove the rotation from the current local space matrix
		*m_matrix *= inverse(oldRotationMatrix);

		// Grab the rotation matrix out of the parent's world space matrix
		auto parentRotationMatrix = eulerRotation(getEulerAngle(m_parent->getWorldSpaceMatrix()));
		// Offset the local space matrix such that it's world space rotation is equal to 'newRotationMatrix'
		*m_matrix *= inverse(parentRotationMatrix) * newRotationMatrix;
	}
	else
		setLocalEulerAngle(newEulerAngle);
}

vec3 Transform::getLocalEulerAngle() const
{
	// Call the static function equivalent and give it this 'm_matrix'
	return getEulerAngle(*m_matrix);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::setLocalEulerAngle(const vec3 &newEulerAngle)
{
	// Call the static function equivalent and give it this 'm_matrix'
	setEulerAngle(m_matrix.get(), newEulerAngle);
}

GLfloat Transform::getScale() const
{
	return getScale(getWorldSpaceMatrix());
}
void Transform::setScale(GLfloat newScale)
{
	if (m_parent != nullptr)
	{
		// Create a new Scale matrix from the vec3 passed in
		auto newScaleMatrix = mat4(1);
		setScale(&newScaleMatrix, newScale);

		// Grab the current scale matrix
		auto oldScaleMatrix = mat4(1);
		setScale(&oldScaleMatrix, getLocalScale());
		// Remove the scale from the current local space matrix
		*m_matrix *= inverse(oldScaleMatrix);

		// Grab the scale matrix out of the parent's world space matrix
		auto parentScaleMatrix = mat4(1);
		setScale(&parentScaleMatrix, getScale(m_parent->getWorldSpaceMatrix()));
		// Offset the local space matrix such that it's world space scale is equal to 'newScaleMatrix'
		*m_matrix *= inverse(parentScaleMatrix) * newScaleMatrix;
	}
	else
		setLocalScale(newScale);
}

GLfloat Transform::getLocalScale() const
{
	return getScale(*m_matrix);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::setLocalScale(GLfloat newScale)
{
	setScale(m_matrix.get(), newScale);
}

mat4 Transform::getWorldSpaceMatrix() const
{
	if (m_parent != nullptr)
		return m_parent->getWorldSpaceMatrix() * *m_matrix;

	return *m_matrix;
}

const mat4 & Transform::getLocalSpaceMatrix() const
{
	return *m_matrix;
}

// ReSharper disable once CppMemberFunctionMayBeConst
// This function is bitwise constant, but not logically constant so it is not marked const
void Transform::setLocalSpaceMatrix(const mat4& newLocalSpaceMatrix)
{
	*m_matrix = newLocalSpaceMatrix;
}

void Transform::drawGizmos() const
{
	drawGizmos(getWorldSpaceMatrix());
}

void Transform::drawGui() const
{
	drawGui(m_matrix.get());
}

Transform::~Transform() { }
