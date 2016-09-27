#include "GameObject.h"

#include <imgui_impl_glfw_gl3.h>

GameObject::GameObject()
{
	m_transform = new Transform(this);
	m_components->push_back(unique_ptr<Transform>(m_transform));
}

void GameObject::draw()
{
	for (auto &component : *m_components)
		component->draw();
}
void GameObject::drawGui()
{
	ImGui::Begin(m_name);
	{
		for (auto &component : *m_components)
			component->drawGui();
	}
	ImGui::End();
}

Transform * GameObject::transform()
{
	return m_transform;
}

const Transform * GameObject::transform() const
{
	return m_transform;
}
