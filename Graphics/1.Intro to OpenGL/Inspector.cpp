#include "Inspector.h"

#include <imgui_impl_glfw_gl3.h>
#include <queue>
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
			ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove);
			{
				s_selected->drawGizmos();
				s_selected->drawGui();
			}
			ImGui::End();
		}

		ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove);
		{
			static GameObject *draggedObject = nullptr;
			static auto dragging = false;
			static GameObject *hoveringObject = nullptr;

			//ImGui::Indent();

			auto indentLevel = 0;
			for (auto &gameObject : Object::findObjectsOfType<GameObject>())
			{
				if (!gameObject->transform()->getParent())
				{
					auto tempQueue = std::stack<unique_ptr<std::queue<GameObject *>>>();

					auto gameObjectStacks = std::stack<std::queue<GameObject *>*>();
					auto gameObjectQueue = std::queue<GameObject *>();
					gameObjectQueue.push(gameObject);

					gameObjectStacks.push(&gameObjectQueue);

					auto currentStack = gameObjectStacks.top();
					auto currentObject = gameObjectQueue.front();

					while (gameObjectStacks.size() > 0)
					{
						while (currentStack->size() > 0)
						{
							auto collapsed = false;
							auto selected = s_selected == currentObject;

							ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
							ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.f, 0.f, 0.f, 0.f));
							if (selected)
								ImGui::PushStyleColor(ImGuiCol_Text, ImColor(143, 143, 200));

							collapsed = ImGui::TreeNodeEx(
								currentObject->getName(),
								currentObject->transform()->getChildren()->size() > 0 ?
								ImGuiTreeNodeFlags_OpenOnArrow :
								ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_OpenOnArrow);
							if (collapsed)
								ImGui::TreePop();

							if (selected)
								ImGui::PopStyleColor();
							ImGui::PopStyleColor(2);

							if (ImGui::IsItemHovered())
							{
								hoveringObject = currentObject;

								if (ImGui::IsMouseClicked(0))
									s_selected = currentObject;

								if (ImGui::IsMouseDragging() && !dragging)
									draggedObject = currentObject;
							}

							if (collapsed && currentObject->transform()->getChildren()->size() > 0)
							{
								ImGui::Indent();
								indentLevel++;

								currentStack->pop();
								tempQueue.push(make_unique<std::queue<GameObject*>>());
								for (auto &child : *currentObject->transform()->getChildren())
									tempQueue.top()->push(child->gameObject());
								gameObjectStacks.push(tempQueue.top().get());

								currentStack = tempQueue.top().get();
								currentObject = tempQueue.top()->front();
							}
							else
							{
								currentStack->pop();
								if (currentStack->size() > 0)
									currentObject = currentStack->front();
							}
						}
						gameObjectStacks.pop();
						if (gameObjectStacks.size() > 0)
						{
							if (indentLevel > 0)
							{
								ImGui::Unindent();
								indentLevel--;
							}

							currentStack = gameObjectStacks.top();
							if (currentStack->size() > 0)
								currentObject = currentStack->front();
						}
					}
				}
				for (auto i = 0; i < indentLevel; ++i, --indentLevel)
					ImGui::Unindent();
			}

			if (draggedObject)
			{
				dragging = true;
				ImGui::SetTooltip("%s", draggedObject->getName());

				if (!ImGui::IsMouseDown(0) && draggedObject != hoveringObject)
				{
					draggedObject->transform()->setParent(
						hoveringObject ? hoveringObject->transform() : nullptr,
						true);
					ImGui::SetWindowFocus("Debug");
					dragging = false;
				}
			}
			else
				ImGui::SetTooltip("");

			if (!dragging)
				draggedObject = nullptr;
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
