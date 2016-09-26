#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec2 inTextureUV;

out vec4 vPosition;
out vec4 vColour;
out vec4 vNormal;
out vec2 vTextureUV;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform mat4 ProjectionViewModel;

void main()
{
	vPosition = ModelMatrix * inPosition;
	vColour = inColour;
	vNormal = NormalMatrix * inNormal;
	vTextureUV = inTextureUV;

    gl_Position = ProjectionViewModel * inPosition;
}