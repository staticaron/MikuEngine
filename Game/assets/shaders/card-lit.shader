#shader vertex
#version 450 core

#include common

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 v_WorldPos;
out vec2 v_UV;
out vec3 v_Normal;
out mat4 v_ModelMtx;

uniform mat4 u_Model;

void main()
{
    gl_Position = cameraProj * cameraView * u_Model * position;
    v_UV = uv;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    v_Normal = normalize(normalMatrix * normal);

    v_WorldPos = vec3(u_Model * position);

    v_ModelMtx = u_Model;
}

#shader fragment
#version 450 core

#include common

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_UV;
in mat4 v_ModelMtx;

uniform sampler2D u_CardMask;
uniform sampler2D u_CardBg;

void main()
{
    vec4 card_mask_rgb = texture(u_CardMask, v_UV);
    vec4 card_bg = texture(u_CardBg, v_UV);

    if (card_mask_rgb.w == 0.0)
        discard;

    vec4 lightRGB = GetLightColor(v_WorldPos, v_Normal);

    color = vec4(card_bg.x * lightRGB.x, card_bg.y * lightRGB.y, card_bg.z * lightRGB.z, card_bg.w);
}
