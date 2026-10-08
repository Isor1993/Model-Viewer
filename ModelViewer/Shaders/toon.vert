#version 460 core

// Projects the vertex through MVP and hands normal, uv and world
// position to the fragment shader.

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec2 v_UV;
out vec3 v_WorldPosition;

void main()
{
	v_Normal = a_Normal;
	v_UV = a_UV;
	v_WorldPosition = (u_Model * vec4(a_Position, 1.0)).xyz;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
