#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec2 v_UV;

uniform mat4 u_MVP = mat4(1.0);

void main()
{
	gl_Position = u_MVP * position;
	v_UV = uv;
}

#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;

in vec2 v_UV;

uniform sampler2D u_Tex;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
