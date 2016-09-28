#include "Component.h"



GameObject * Component::gameObject()
{
	return m_gameObject;
}
const GameObject * Component::gameObject() const
{
	return m_gameObject;
}