#shader vertex
#version 450 core

#include common

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec3 v_WorldPos;
out vec2 v_UV;
out mat3 v_TBN;

uniform mat4 u_Model;

void main()
{
    gl_Position = cameraProj * cameraView * u_Model * position;
    v_UV = uv;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));

    vec3 v_Normal = normalize(normalMatrix * normal);
    vec3 v_Tangent = normalize(mat3(u_Model) * tangent);
    vec3 v_BiTangent = normalize(mat3(u_Model) * bitangent);

    v_TBN = mat3(v_Tangent, v_BiTangent, v_Normal);

    v_WorldPos = vec3(u_Model * position);
}

#shader fragment
#version 450 core

#include common

// output color
layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec2 v_UV;
in mat3 v_TBN;

uniform sampler2D u_Gradient;

uniform vec2 u_GradientUVMultiplier;

void main()
{
    // Gradient Calculation
    vec3 viewDirectionTangentSpace = transpose(v_TBN) * normalize(cameraPos.xyz - v_WorldPos);
    vec2 uvMovedByViewDirection = vec2(v_UV.x * u_GradientUVMultiplier.x + viewDirectionTangentSpace.x, v_UV.y * u_GradientUVMultiplier.y + viewDirectionTangentSpace.y);
    vec4 gradientColor = texture(u_Gradient, uvMovedByViewDirection);

    color = gradientColor;
}
