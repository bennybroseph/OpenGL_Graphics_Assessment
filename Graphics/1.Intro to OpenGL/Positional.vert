#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec4 inNormal;

out vec4 vPosition;
out vec4 vColour;
out vec4 vNormal;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform mat4 ProjectionViewModel;

void main()
{
	vPosition = ModelMatrix * inPosition;
	vColour = inPosition;
	vNormal = NormalMatrix * inNormal;

    gl_Position = ProjectionViewModel * inPosition;
}