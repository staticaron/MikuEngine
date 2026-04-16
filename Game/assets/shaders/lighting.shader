#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout( location = 1 ) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(std140, binding = 0) uniform u_Matrices
{
	mat4 projection;
	mat4 view;
};

out vec2 v_UV;
out vec3 v_Normal;

uniform mat4 u_Model;

void main()
{
	gl_Position = projection * view * u_Model * position;
	v_UV = uv;
	v_Normal = normal;
}

#shader fragment
#version 450 core 

// output color
layout(location = 0) out vec4 color;

layout( std140, binding = 1) uniform m_LightingData
{
	vec4 lightPos;
	vec4 lightDir;
	vec4 lightColor;
	float lightIntensity;
};

in vec3 v_Normal;
in vec2 v_UV;

uniform sampler2D u_Tex;

void main()
{
	vec4 tex = texture(u_Tex, v_UV);
	vec4 normal_color = vec4( v_Normal.x, v_Normal.y, v_Normal.z, 1.0);

	// color = vec4( tex.x + normal_color.x * 0.3, tex.y + normal_color.y * 0.3, tex.z + normal_color.z * 0.3, tex.w);
	color = vec4( tex.x, tex.y, tex.z, tex.w);
}
