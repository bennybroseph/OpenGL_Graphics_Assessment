#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#pragma once

#include "Object.h"

class GameObject;

class Component;

typedef unique_ptr<Component> ComponentPtrU;
typedef shared_ptr<Component> ComponentPtrS;
typedef weak_ptr<Component> ComponentPtrW;

class Component : public Object
{

	friend class GameObject;

public:

	Component() : Object() { }

	GameObject * gameObject();
	const GameObject * gameObject() const;

	virtual ~Component() { }

protected:

	GameObject *m_gameObject = nullptr;

};


#endif // _COMPONENT_H_
