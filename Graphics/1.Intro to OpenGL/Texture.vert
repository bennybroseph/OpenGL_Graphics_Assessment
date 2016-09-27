#version 410

layout(location=0) in vec4 inPosition;
layout(location=3) in vec2 inTextureUV;

out vec2 vTextureUV;

uniform mat4 ProjectionViewModel;
void main()
{
	vTextureUV = inTextureUV;
	gl_Position = ProjectionViewModel * inPosition;
}