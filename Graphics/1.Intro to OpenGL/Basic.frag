#version 410

in vec4 vColour;

out vec4 outColour;

uniform vec4 MaterialAmbient = vec4(1.f, 1.f, 1.f, 1.f);

void main()
{
	outColour = vColour * MaterialAmbient;
}