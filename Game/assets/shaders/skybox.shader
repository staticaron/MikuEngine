#shader vertex
#version 450 core

#include common

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec3 v_LocalPos;

uniform mat4 u_Model;

void main()
{
    mat4 camView = mat4(mat3(cameraView));
    vec4 pos = cameraProj * camView * position;
    gl_Position = pos.xyww;

    v_LocalPos = normalize(position.xyz);
}

#shader fragment
#version 450 core

// output color
layout(location = 0) out vec4 color;

in vec3 v_LocalPos;

uniform samplerCube u_Cubemap;

void main()
{
    vec4 rgb = texture(u_Cubemap, v_LocalPos);
    color = rgb;
}
