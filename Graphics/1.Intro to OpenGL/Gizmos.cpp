#include "Gizmos.h"

#include "Camera.h"

namespace Gizmos
{
	int init()
	{
		Plane::init();
		Cube::init();
		Sphere::init();
		Line::init();

		return 0;
	}

	int drawSphere(const mat4 &transform, const vec4 &colour, GLboolean drawWireFrame)
	{
		auto newSphere = Sphere::create();

		//newSphere->transform()->setLocalSpaceMatrix(transform);

		newSphere->setMaterialColour(colour);

		newSphere->m_shouldDrawWireFrame = drawWireFrame;

		//newSphere->draw();

		return 0;
	}
	// TODO: This function is so expensive and needs to be rewritten...please only buffer once somehow
	int drawLine(
		const vec3 &start,
		const vec3 &end,
		const vec4 &colourStart,
		const vec4 &colourEnd,
		GLfloat width)
	{
		auto parsedEndColour = colourEnd;
		if (parsedEndColour == vec4(-1))
			parsedEndColour = colourStart;

		auto newLine = Line::create(start, end, colourStart, parsedEndColour);
		newLine->drawModel(mat4(1));

		return 0;
	}

	int drawGrid(const vec3 &center, const vec2 &spacing, const vec2 &segments, GLfloat lineWidth)
	{
		struct Line
		{
			vec3 start, end;
			vec4 colour;
		};

		auto lines = vector<Line>();

		auto originColour = vec4(1);
		auto otherColour = vec4(0.75f, 0.75f, 0.75f, 1.f);

		for (auto i = 0; i <= segments.y; ++i)
		{
			lines.push_back(
			{
				vec3(center.x - spacing.x * segments.x, 0.f, i * spacing.y),
				vec3(center.x + spacing.x * segments.x, 0.f, i * spacing.y),
				i == 0 ? originColour : otherColour
			});
			if (i != 0)
			{
				lines.push_back(
				{
					vec3(center.x - spacing.x * segments.x, 0.f, -i * spacing.y),
					vec3(center.x + spacing.x * segments.x, 0.f, -i * spacing.y),
					otherColour
				});
			}
		}
		for (auto i = 0; i <= segments.x; ++i)
		{
			lines.push_back(
			{
				vec3(i * spacing.x, 0.f, center.y - spacing.y * segments.y),
				vec3(i * spacing.x, 0.f, center.y + spacing.y * segments.y),
				i == 0 ? originColour : otherColour
			});
			if (i != 0)
			{
				lines.push_back(
				{
					vec3(-i * spacing.x, 0.f, center.y - spacing.y * segments.y),
					vec3(-i * spacing.x, 0.f, center.y + spacing.y * segments.y),
					otherColour
				});
			}
		}

		for (auto line : lines)
			Gizmos::drawLine(line.start, line.end, line.colour);

		return 0;
	}

	int quit()
	{
		Plane::quit();
		Cube::quit();
		Sphere::quit();
		Line::quit();

		return 0;
	}
}
