#version 410

in vec4 vColour;

out vec4 outColour;

// Inspector Color()
uniform vec4 MaterialAmbient = vec4(1.f, 1.f, 1.f, 1.f);
// Inspector Min(-2) Max(5)
uniform vec3 AVector3 = vec3(0, 0, 1);
// Inspector Step(10.f)
uniform vec2 AVector2 = vec2(5, 2);
// Inspector Range(-1, 500)
uniform float Float = 5.f;

void main()
{
	outColour = vColour * MaterialAmbient;
}