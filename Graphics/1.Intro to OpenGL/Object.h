#ifndef _OBJECT_H_
#define _OBJECT_H_
#pragma once

#include <gl_core_4_4.h>

#include "MasterHeader.h"

class Object;

typedef unique_ptr<Object> ObjectPtrU;
typedef shared_ptr<Object> ObjectPtrS;
typedef weak_ptr<Object> ObjectPtrW;

class Object
{

	static GLuint genID();

public:

	Object();

	virtual void draw() const { }
	virtual void drawGui() const { }

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
		{
			auto returnValue = dynamic_cast<T *>(object);
			if (returnValue)
				objects.push_back(returnValue);
		}

		return objects;
	}

protected:

	unique_ptr<GLchar> m_name = unique_ptr<GLchar>(new GLchar[255]);
	const GLuint m_id = genID();

private:

	static vectorPtrU<Object *> s_objects;

};

#endif // _OBJECT_H_
