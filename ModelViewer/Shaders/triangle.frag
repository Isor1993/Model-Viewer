#version 460 core

const float F_SHADOW_EDGE = 0.4;
const float F_EDGE_SOFTNESS = 0.03;
const vec3 VEC3_SHADOW_TINT = vec3(0.30, 0.35, 0.50);

in vec3 v_Normal;
in vec2 v_UV;
in vec3 v_WorldPosition;

const vec3 VEC3_HORIZON_COLOR = vec3(0.90, 0.95, 0.98);
const float F_EDGE_FADE_START = 20.0;
const float F_EDGE_FADE_END = 45.0;

uniform sampler2D u_Texture;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(v_Normal);
	vec3 toSun = normalize(- u_LightDirection);
		
	float brightness = dot(normal, toSun);
	float edgeBlend = smoothstep(F_SHADOW_EDGE - F_EDGE_SOFTNESS, F_SHADOW_EDGE + F_EDGE_SOFTNESS, brightness);
	vec3 shade = mix(VEC3_SHADOW_TINT, vec3(1.0), edgeBlend);
	

	vec4 textureColor = texture(u_Texture,v_UV);
	vec3 litColor = textureColor.rgb * u_LightColor * shade;

	float edgeFade = smoothstep(F_EDGE_FADE_START, F_EDGE_FADE_END, max(abs(v_WorldPosition.x), abs(v_WorldPosition.z)));
	litColor = mix(litColor, VEC3_HORIZON_COLOR, edgeFade);

	FragColor = vec4(litColor,1.0);
}