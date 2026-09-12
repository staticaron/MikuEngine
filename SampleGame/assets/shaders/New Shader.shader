#shader vertex
#version 450 core

#include common

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec2 v_UV;

uniform mat4 u_Model;

void main()
{
    gl_Position = cameraProj * cameraView * u_Model * position;
    v_UV = uv;
}

#shader fragment
#version 450 core

// output color
layout(location = 0) out vec4 color;

in vec2 v_UV;

uniform sampler2D u_MainTex;

void main()
{
    vec4 rgb = texture(u_MainTex, v_UV);
    color = rgb;
}
