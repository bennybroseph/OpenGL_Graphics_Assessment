#version 410

in vec4 vColour;

out vec4 outColour;

uniform vec4 MaterialAmbient;

void main()
{
	outColour = vColour * MaterialAmbient;
}