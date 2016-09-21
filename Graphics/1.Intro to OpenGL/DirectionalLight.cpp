#include "DirectionalLight.h"

#include <vector>

#include "Gizmos.h"

void DirectionalLight::draw()
{
	struct Line { vec3 start, end; };

	auto lines = vector<Line>();

	auto startMatrix = m_transform.getWorldSpaceMatrix();
	auto endMatrix = m_transform.getWorldSpaceMatrix() * translate(vec3(1.f, 0.f, 0.f));

	auto lineCount = 5;
	for (auto i = 0; i < lineCount; ++i)
	{
		for (auto j = 0; j < lineCount; ++j)
		{
			auto newStartMatrix =
				startMatrix
				* translate(vec3(0.f, 0.25f * i / (lineCount - 1.f) - 0.125f, 0.25f * j / (lineCount - 1.f) - 0.125f));
			auto newEndMatrix =
				endMatrix
				* translate(vec3(0.f, 0.25f * i / (lineCount - 1.f) - 0.125f, 0.25f * j / (lineCount - 1.f) - 0.125f));

			lines.push_back(
			{
				vec3(newStartMatrix[3].x, newStartMatrix[3].y, newStartMatrix[3].z),
				vec3(newEndMatrix[3].x, newEndMatrix[3].y, newEndMatrix[3].z)
			});
		}
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
