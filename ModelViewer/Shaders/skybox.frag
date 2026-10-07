#version 460 core

in vec3 v_Direction;

uniform samplerCube u_Skybox;

out vec4 FragColor;

void main()
{
	FragColor = texture(u_Skybox, v_Direction);
}