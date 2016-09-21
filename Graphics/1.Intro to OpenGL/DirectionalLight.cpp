#include "DirectionalLight.h"

#include <vector>

#include "Gizmos.h"

void DirectionalLight::draw()
{
	struct Line { vec3 start, end; };

	auto lines = vector<Line>();

	auto startMatrix = m_transform.getWorldSpaceMatrix();
	auto endMatrix = m_transform.getWorldSpaceMatrix() * translate(vec3(0.f, 0.f, 0.75f));

	auto lineCount = 15.f;
	for (auto i = 0.f; i < lineCount; ++i)
	{
		auto theta = 2 * glm::pi<float>() * i / lineCount;

		auto newStartMatrix = startMatrix * translate(vec3(0.25f * cos(theta), 0.25f * sin(theta), 0.f));
		auto newEndMatrix = endMatrix * translate(vec3(0.25f * cos(theta), 0.25f * sin(theta), 0.f));

		lines.push_back(
		{
			vec3(newStartMatrix[3].x, newStartMatrix[3].y, newStartMatrix[3].z),
			vec3(newEndMatrix[3].x, newEndMatrix[3].y, newEndMatrix[3].z)
		});
	}

	for (auto line : lines)
	{
		Gizmos::drawLine(line.start, line.end, vec4(m_diffuse, 1.f));
	}
	m_transform.draw();
}

DirectionalLight::~DirectionalLight()
{

}