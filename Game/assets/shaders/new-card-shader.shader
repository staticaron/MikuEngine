#shader vertex
#version 450 core

#include common

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 v_WorldPos;
out vec2 v_UV;
out vec3 v_WorldNormal;
out mat4 v_ModelMtx;

uniform mat4 u_Model;

void main()
{
    gl_Position = cameraProj * cameraView * u_Model * position;
    v_UV = uv;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    v_WorldNormal = normalize(normalMatrix * normal);

    v_WorldPos = vec3(u_Model * position);

    v_ModelMtx = u_Model;
}

#shader fragment
#version 450 core

#include common

// output color
layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_UV;
in mat4 v_ModelMtx;

uniform sampler2D u_CardMask;
uniform sampler2D u_Bg;
uniform sampler2D u_CardBg;
uniform sampler2D u_Char;
uniform sampler2D u_CardFg;

void main()
{
    vec4 card_mask_rgb = texture(u_CardMask, v_UV);

    float max_offset_char = 0.15;
    float max_offset_bg = 0.05;

    // Calculate the offset according to the view direction
    vec3 viewDir = normalize(-cameraDir.xyz);
    float dotNV = dot(normalize(v_WorldNormal), viewDir);
    float glazingAngle = acos(clamp(dotNV, 0.0, 1.0));

    vec3 modelRight = GetRightFromMatrix(v_ModelMtx);
    vec3 modelUp = GetUpFromMatrix(v_ModelMtx);

    float isRight = dot(modelRight, viewDir);
    float isUp = dot(modelUp, viewDir);

    vec2 offset = vec2(0.0);

    offset.x = -isRight * 0.5 * glazingAngle;
    offset.y = isUp * 0.5 * glazingAngle;

    // UV used by character texture
    vec2 ch_UV = v_UV;

    // UV used by background elements
    vec2 bg_UV = v_UV * (1 - 2 * max_offset_bg);
    bg_UV += vec2(max_offset_bg);

    // apply the offset to both the UVs
    ch_UV = vec2(ch_UV.x - offset.x * max_offset_char, ch_UV.y - offset.y * max_offset_char);
    bg_UV = vec2(bg_UV.x + offset.x * max_offset_bg, bg_UV.y + offset.y * max_offset_bg);

    // texture colors
    vec4 bg = texture(u_Bg, bg_UV);
    vec4 ch = texture(u_Char, ch_UV);
    vec4 card_bg = texture(u_CardBg, v_UV);
    vec4 card_fg = texture(u_CardFg, v_UV);

    // apply the texture colors based on the opacity and order
    vec4 rgb = bg;
    rgb = mix(rgb, card_bg, card_bg.w);
    rgb = mix(rgb, ch, ch.w);
    rgb = mix(rgb, card_fg, card_fg.w);

    vec2 diffSpec = GetDiffuseSpecular(v_WorldPos, v_WorldNormal);

    float intensity = max(diffSpec.x + diffSpec.y * float(ch.w > 0.5 || card_bg.w > 0.5 || card_fg.w > 0.5) + lightAmbientIntensity, 0.0);

    vec4 lightRGB = lightColor * intensity;

    color = vec4(rgb.x * lightRGB.x, rgb.y * lightRGB.y, rgb.z * lightRGB.z, card_mask_rgb.w);
}
