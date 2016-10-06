#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec2 inTextureUV;

out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec2 vTextureUV;

uniform mat4 ProjectionViewModel;

void main()
{
	vNormal = inNormal.xyz;
	vTangent = inTangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTextureUV = inTextureUV;

	gl_Position = ProjectionViewModel * inPosition;
}