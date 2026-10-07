#version 460 core

in vec3 v_Normal;
in vec2 v_UV;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	FragColor = texture(u_Texture,v_UV);
}