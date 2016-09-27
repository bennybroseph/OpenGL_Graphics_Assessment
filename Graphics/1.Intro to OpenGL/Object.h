#ifndef _OBJECT_H_
#define _OBJECT_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"

class Object
{

	static GLuint genID();

public:

	Object();

	const GLchar * getName() const;
	void setName(const GLchar *newName);

	GLuint getID() const;

	virtual ~Object();

	template<typename T>
	static T * findObjectOfType()
	{
		for (auto &object : *s_objects)
			if (dynamic_cast<T *>(object))
				return object;

		return nullptr;
	}

	template<typename T>
	static vector<T *>  findObjectsOfType()
	{
		auto objects = vector<T *>();

		for (auto &object : *s_objects)
			if (dynamic_cast<T *>(object))
				objects.push_back(object);

		return objects;
	}

protected:

	const GLchar *m_name = "";
	const GLuint m_id = genID();

private:

	static vectorPtrU<Object *> s_objects;

};

#endif // _OBJECT_H_
