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
out vec4 v_DebugColor;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	gl_Position = projection * view * u_Model * position;
	v_UV = uv;

	v_DebugColor = vec4( projection[0][0], projection[1][1], projection[2][2], 1.0);
}

#shader fragment
#version 450 core 

layout(location = 0) out vec4 color;

in vec2 v_UV;
in vec4 v_DebugColor;

uniform sampler2D u_Tex;

void main()
{
	vec4 rgb = texture(u_Tex, v_UV);
	color = rgb;
}
