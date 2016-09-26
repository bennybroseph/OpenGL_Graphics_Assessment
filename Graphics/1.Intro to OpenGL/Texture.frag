#version 410
in vec2 vTextureUV;

out vec4 FragColor;

uniform sampler2D diffuseMap;

void main()
{
	FragColor = texture(diffuseMap, vTextureUV);
}