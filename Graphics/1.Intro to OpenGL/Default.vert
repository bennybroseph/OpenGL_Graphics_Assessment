#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec4 inNormal;

out vec4 fragPosition;
out vec4 fragColour;
out vec4 fragNormal;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform mat4 ProjectionViewModel;

void main()
{
	fragPosition = ModelMatrix * inPosition;
	fragColour = inColour;
	fragNormal = NormalMatrix * inNormal;

    gl_Position = ProjectionViewModel * inPosition;
}