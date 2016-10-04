#version 410

in vec2 vTextureUV;

out vec4 out_color;

uniform sampler2D perlin_texture;

void main()
{
	out_color = texture(perlin_texture, vTextureUV).rrrr;
	out_color.a = 1;
}