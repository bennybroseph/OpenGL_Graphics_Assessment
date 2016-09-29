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
			ImGui::Begin("Inspector");
			{
				s_selected->drawGizmos();
				s_selected->drawGui();
			}
			ImGui::End();
		}

		ImGui::Begin("Hierarchy");
		{
			static GameObject *draggedObject = nullptr;
			static auto dragging = false;
			static GameObject *hoveringObject = nullptr;

			auto indentLevel = 0;
			for (auto &gameObject : Object::findObjectsOfType<GameObject>())
			{
				if (!gameObject->transform()->getParent())
				{
					auto gameObjectStacks = std::stack<std::queue<GameObject *>*>();
					auto gameObjectStack = std::queue<GameObject *>();
					gameObjectStack.push(gameObject);

					gameObjectStacks.push(&gameObjectStack);

					auto currentStack = gameObjectStacks.top();
					auto currentObject = gameObjectStack.front();

					while (gameObjectStacks.size() > 0)
					{
						while (currentStack->size() > 0)
						{
							auto collapsed = ImGui::CollapsingHeader(
								currentObject->getName(),
								nullptr,
								s_selected == currentObject ? true : false);

							if (ImGui::IsItemHovered())
								hoveringObject = currentObject;

							if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
								s_selected = currentObject;

							if (ImGui::IsItemHovered() && ImGui::IsMouseDragging())
								draggedObject = currentObject;

							if (collapsed && currentObject->transform()->getChildren()->size() > 0)
							{
								ImGui::Indent();
								indentLevel++;

								currentStack->pop();
								auto tempStack = new std::queue<GameObject *>();
								for (auto &child : *currentObject->transform()->getChildren())
									tempStack->push(child->gameObject());
								gameObjectStacks.push(tempStack);

								currentStack = tempStack;
								currentObject = tempStack->front();
							}
							else
							{
								/*if (indentLevel > 0 &&
									collapsed &&
									currentObject->transform()->getChildren()->size() > 0)
								{
									ImGui::Unindent();
									indentLevel--;
								}*/

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
