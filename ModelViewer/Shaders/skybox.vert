#version 460 core

// Skybox: view arrives without translation (mat3 trim in C++), and
// gl_Position.xyww pins the depth to 1.0 — drawn last with GL_LEQUAL.

layout (location = 0) in vec3 a_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Direction;

void main()
{
	v_Direction = a_Position;

	vec4 position = u_Projection * u_View * vec4(a_Position, 1.0);
	gl_Position = position.xyww;
}