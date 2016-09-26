#version 410

in vec4 vColour;
in vec2 vTextureUV;

out vec4 outColour;

uniform vec3 LightAmbient;

uniform vec3 MaterialAmbient;

uniform sampler2D diffuseMap;

void main()
{
	outColour = vColour * (vec4(LightAmbient * MaterialAmbient, 1) + texture(diffuseMap, vTextureUV));
}