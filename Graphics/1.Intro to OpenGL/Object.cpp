#include "Object.h"

vectorPtrU<Object *> Object::s_objects = make_unique<vector<Object *>>();

Object::Object()
{
	if (s_objects.get() == nullptr)
		s_objects.reset(new vector<Object *>);

	s_objects->push_back(this);	// Add itself to the list of objects
}

const GLchar * Object::getName() const
{
	return m_name.get();
}
void Object::setName(const GLchar *newName) const
{
	_memccpy(m_name.get(), newName, 0, 255);
}

GLuint Object::getID() const
{
	return m_id;
}

Object::~Object()
{
	s_objects->erase(find(s_objects->begin(), s_objects->end(), this));	// Remove itself from the list of objects

	if (s_objects->size() == 0)
		s_objects.reset();
}

GLuint Object::genID()
{
	static GLuint counter = 0;

	return counter++;
}
