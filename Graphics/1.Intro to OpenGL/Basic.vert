// Used for drawing primitives which do not have a model only a mesh
#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;

out vec4 vPosition;
out vec4 vColour;

uniform mat4 ProjectionViewModel;

void main()
{
	vPosition = ModelMatrix * inPosition;
	vColour = inColour;

    gl_Position = ProjectionViewModel * inPosition;
}