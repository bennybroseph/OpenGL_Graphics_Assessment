#version 410

in vec4 vColour;
in vec2 vTextureUV;

out vec4 outColour;

uniform vec3 LightAmbient;

// Inspector Color()
uniform vec3 MaterialAmbient = vec3(1.f, 1.f, 1.f);

uniform sampler2D diffuseMap;

void main()
{
	outColour = vColour * (vec4(MaterialAmbient, 1));
}