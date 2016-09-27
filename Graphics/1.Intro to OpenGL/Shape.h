#ifndef _SHAPE_H_
#define _SHAPE_H_
#pragma once

#include "Model.h"

class Shape;

typedef unique_ptr<Shape> ShapePtrU;
typedef shared_ptr<Shape> ShapePtrS;
typedef weak_ptr<Shape> ShapePtrW;

class Shape
{
public:

	virtual void draw();

	Model * model();
	const Model * model() const;

	virtual ~Shape();

protected:

	const ModelPtrU m_model = make_unique<Model>();
};

#endif // _SHAPE_H_