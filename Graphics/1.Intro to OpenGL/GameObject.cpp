#include "GameObject.h"

#include <imgui_impl_glfw_gl3.h>

GameObject::GameObject() : Object()
{
	m_transform = new Transform();
	m_transform->m_gameObject = this;
	m_components->push_back(unique_ptr<Transform>(m_transform));
}

void GameObject::draw() const
{
	for (auto &component : *m_components)
		component->draw();
}
void GameObject::drawGui() const
{
	ImGui::PushID(this);
	ImGui::BeginChild(m_name->c_str());
	{
		GLchar newName[255];
		_memccpy(newName, m_name->c_str(), 0, 255);

		ImGui::InputText("Name", newName, 255, ImGuiInputTextFlags_EnterReturnsTrue);

		if (ImGui::IsItemActive())
			*m_name = newName + '\0';

		for (auto &component : *m_components)
			component->drawGui();
	}
	ImGui::EndChild();
	ImGui::PopID();
}

Transform * GameObject::transform()
{
	return m_transform;
}

const Transform * GameObject::transform() const
{
	return m_transform;
}
