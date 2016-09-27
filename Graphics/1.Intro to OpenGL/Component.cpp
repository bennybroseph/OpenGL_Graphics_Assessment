#include "Component.h"



Component::Component(GameObject * parent)
{
	m_gameObject = parent;
}



GameObject * Component::gameObject()
{
	return m_gameObject;
}
const GameObject * Component::gameObject() const
{
	return m_gameObject;
}

Component::~Component()
{
}
