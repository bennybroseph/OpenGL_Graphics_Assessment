#version 410

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec2 inTextureUV;

out vec3 vPosition;
out vec3 vColour;
out vec3 vNormal;

out vec3 vTangent;
out vec3 vBiTangent;

out vec2 vTextureUV;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform mat4 ProjectionViewModel;

uniform sampler2D perlin_texture;
// Editable
uniform float perlinCoefficient = 0.25f;

void main()
{
	vPosition = (ModelMatrix * inPosition).xyz;
	vColour = inColour.xyz;
	vNormal = (NormalMatrix * inNormal).xyz;
	vTangent = inTangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTextureUV = inTextureUV;

	vec4 newPosition = inPosition;
	newPosition.y = (texture(perlin_texture, vTextureUV).r) * perlinCoefficient;

	gl_Position = ProjectionViewModel * newPosition;
}