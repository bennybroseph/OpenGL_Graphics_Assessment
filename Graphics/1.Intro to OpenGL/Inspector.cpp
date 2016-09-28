#include "Inspector.h"

#include <imgui_impl_glfw_gl3.h>
#include <stack>

namespace Editor
{
	Object *Inspector::s_selected = nullptr;

	int Inspector::init()
	{
		return 0;
	}

	void Inspector::drawGui()
	{
		if (s_selected)
		{
			ImGui::Begin("Inspector");
			{
				s_selected->draw();
				s_selected->drawGui();
			}
			ImGui::End();
		}

		ImGui::Begin("Hierarchy");
		{
			for (auto &gameObject : Object::findObjectsOfType<GameObject>())
			{
				if (!gameObject->transform()->getParent())
				{
					auto collapsed = ImGui::CollapsingHeader(gameObject->getName(), nullptr, false);

					if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
						s_selected = gameObject;

					if(collapsed)
					{
						auto gameObjects = std::stack<GameObject *>();
						gameObjects.push(gameObject);

						auto currentObject = gameObjects.top();
						auto indentCount = 0;
						while (gameObjects.size() > 0)
						{
							indentCount++;
							ImGui::Indent();
							for (
								auto iter = currentObject->transform()->getChildren()->rbegin();
								iter != currentObject->transform()->getChildren()->rend(); ++iter)
							{
								auto childCollapsed =
									ImGui::CollapsingHeader((*iter)->gameObject()->getName(), nullptr, false);

								if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
									s_selected = (*iter)->gameObject();

								if (childCollapsed)
									continue;

								gameObjects.push((*iter)->gameObject());
							}

							currentObject = gameObjects.top();
							gameObjects.pop();
						}

						for (auto i = 0; i < indentCount; ++i)
							ImGui::Unindent();
					}
				}
			}
		}
		ImGui::End();
	}

	Object * Inspector::selected()
	{
		return s_selected;
	}

	int Inspector::quit()
	{
		return 0;
	}
}
