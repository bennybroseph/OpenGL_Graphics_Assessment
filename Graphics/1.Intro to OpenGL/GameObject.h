#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#pragma once

#include "Transform.h"

class GameObject;

typedef unique_ptr<GameObject> GameObjectPtrU;
typedef shared_ptr<GameObject> GameObjectPtrS;
typedef weak_ptr<GameObject> GameObjectPtrW;

class GameObject : public Object
{
public:

	GameObject();

	void draw() const override;
	void drawGizmos() const override;
	void drawGui() const override;

	template<typename T>
	int addComponent()
	{
		return addComponent(new T());
	}
	template<typename T>
	int addComponent(T *component)
	{
		return addComponent(unique_ptr<T>(component));
	}
	template<typename T>
	int addComponent(unique_ptr<T> componentPtr)
	{
		if (dynamic_cast<Component *>(componentPtr.get()))
		{
			componentPtr->m_gameObject = this;
			componentPtr->init();
			m_components->push_back(std::move(componentPtr));
			return 0;
		}

		return -1;
	}

	template<typename T>
	T * getComponent()
	{
		for (auto &component : *m_components)
		{
			auto *returnComponent = dynamic_cast<T *>(component.get());
			if (returnComponent)
				return returnComponent;
		}

		return nullptr;
	}
	template<typename T>
	vector<T *>  getComponents()
	{
		auto objects = vector<T *>();

		for (auto &component : *m_components)
		{
			auto *returnComponent = dynamic_cast<T *>(component.get());
			if (returnComponent)
				objects.push_back(returnComponent);
		}

		return objects;
	}

	Transform * transform();
	const Transform * transform() const;

	~GameObject() { }

private:

	vectorPtrU<ComponentPtrU> m_components = make_unique<vector<ComponentPtrU>>();

	Transform *m_transform = nullptr;

};
#endif // _GAMEOBJECT_H_