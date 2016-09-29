#version 410

in vec4 vColour;

out vec4 outColour;

uniform vec3 MaterialAmbient;

void main()
{
	outColour = vColour * (vec4(MaterialAmbient, 1));
}