#version 410

in vec4 fragColour;

out vec4 outColour;

uniform vec4 vMatColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	outColour = fragColour * vMatColor;
}