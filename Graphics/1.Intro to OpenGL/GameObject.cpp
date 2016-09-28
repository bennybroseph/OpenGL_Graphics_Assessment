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

void GameObject::drawGizmos() const
{
	for (auto &component : *m_components)
		component->drawGizmos();
}

void GameObject::drawGui() const
{
	ImGui::PushID(this);
	ImGui::BeginGroup();
	{
		ImGui::InputText("Name", m_name.get(), 255, ImGuiInputTextFlags_EnterReturnsTrue);

		for (auto &component : *m_components)
		{
			ImGui::PushID(component.get());
			{
				if (ImGui::CollapsingHeader(component->getName(), nullptr, true, true))
					component->drawGui();
			}
			ImGui::PopID();
		}
	}
	ImGui::EndGroup();
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
