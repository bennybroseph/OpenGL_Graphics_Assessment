#include "Shape.h"


void Shape::draw()
{
	m_model->draw();
}

Model * Shape::model()
{
	return m_model.get();
}
const Model* Shape::model() const
{
	return m_model.get();
}

Shape::~Shape() { }
