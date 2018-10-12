#version 430
out vec4 fragColor;
in vec2 texCords2;
in vec4 Color;
uniform sampler2D diffuse;
void main()
{
	fragColor = texture(diffuse, texCords2);
}