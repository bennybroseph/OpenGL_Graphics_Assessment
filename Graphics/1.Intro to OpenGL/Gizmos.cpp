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

	int drawCube(const mat4 &transform, const vec4 &colour, GLboolean drawWireFrame)
	{
		auto newCube = Cube::getMesh();
		if (!newCube)
			return -1;

		newCube->drawMesh(transform, colour, true, drawWireFrame, GL_TRIANGLE_STRIP);

		return 0;
	}

	int drawPlane(const mat4 &transform, const vec4 &colour, GLboolean drawWireFrame)
	{
		auto newPlane = Plane::getMesh();
		if (!newPlane)
			return -1;

		newPlane->drawMesh(transform, colour, true, drawWireFrame, GL_TRIANGLES);

		return 0;
	}

	int drawSphere(const mat4 &transform, const vec4 &colour, GLboolean drawWireFrame)
	{
		auto newSphere = Sphere::getMesh();
		if (!newSphere)
			return -1;

		newSphere->drawMesh(transform, colour, true, drawWireFrame, GL_TRIANGLE_STRIP);

		return 0;
	}
	// TODO: This function is so expensive and needs to be rewritten...
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

		auto newLine = Line::getMesh(start, end, colourStart, parsedEndColour);
		if (!newLine)
			return -1;

		glLineWidth(width);
		{
			newLine->drawMesh(mat4(1), vec4(1), false, true, GL_LINES);
		}
		glLineWidth(1.f);

		return 0;
	}
	// TODO: This function is so expensive and needs to be rewritten...
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
