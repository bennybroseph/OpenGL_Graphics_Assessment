#ifndef _TINYOBJLOADER_H_
#define _TINYOBJLOADER_H_
#pragma once

#include "BaseApplication.h"
#include "GameObject.h"


#include <tiny_obj_loader.h>

class TinyObjLoader : public BaseApplication
{

	struct OpenGLInfo
	{
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
		unsigned int m_index_count;
	};

public:

	int startup() override;

private:

	// Inherited via BaseApplication
	void shutdown() override;
	void parseInput() override;
	void update() override;
	void lateUpdate() override;
	void draw() override;

	void createOpenGLBuffers(tinyobj::attrib_t &attribs, vector<tinyobj::shape_t>& shapes);

	GameObjectPtrU m_camera = nullptr;

	vector<OpenGLInfo> m_gl_info = vector<OpenGLInfo>();

};


#endif // _TINYOBJLOADER_H_
