#version 410

in vec4 vColour;

out vec4 fragColor;

uniform vec4 vMatColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	fragColor = vColour * vMatColor;
}