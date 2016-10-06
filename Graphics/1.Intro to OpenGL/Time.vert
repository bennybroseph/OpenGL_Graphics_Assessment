// Used for drawing primitives which do not have a model only a mesh
#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;

out vec4 vColour;

uniform mat4 ProjectionViewModel;

uniform float time;
// Inspector Min(0) Max(10)
uniform float heightScale = 1.f;

void main()
{
	vColour = inColour;

	vec4 position = inPosition;
	position.y += sin( time + inPosition.x ) * heightScale;
    gl_Position = ProjectionViewModel * position;
}