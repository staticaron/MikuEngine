#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

layout(std140, binding = 0) uniform u_Matrices
{
	mat4 projection;
	mat4 view;
};

out vec2 v_UV;

uniform mat4 u_Model;

void main()
{
	gl_Position = projection * view * u_Model * position;
	v_UV = uv;
}

#shader fragment
#version 450 core 

// output color
layout(location = 0) out vec4 color;

layout( std140, binding = 1) uniform m_LightingData
{
	vec3 lightColor;
	float lightIntensity;
};

in vec2 v_UV;

uniform sampler2D u_Tex;

void main()
{
	color = vec4( lightColor.x * lightIntensity, lightColor.y, lightColor.z, 1.0);
}
