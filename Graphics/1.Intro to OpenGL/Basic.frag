#version 410

in vec4 vColour;

out vec4 outColour;

// Inspector Color()
uniform vec4 MaterialAmbient = vec4(1.f, 1.f, 1.f, 1.f);
// Inspector
uniform vec3 AVector3 = vec3(0, 0, 1);
// Inspector
uniform vec2 AVector2 = vec2(5, 2);

void main()
{
	outColour = vColour * MaterialAmbient;
}