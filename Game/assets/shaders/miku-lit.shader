#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(std140, binding = 0) uniform u_Matrices
{
    mat4 cameraProj;
    mat4 cameraView;

    vec4 cameraPos;
    vec4 cameraDir;
};

out vec3 v_WorldPos;
out vec2 v_UV;
out vec3 v_Normal;

uniform mat4 u_Model;

void main()
{
    gl_Position = cameraProj * cameraView * u_Model * position;
    v_UV = uv;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    v_Normal = normalize(normalMatrix * normal);

    v_WorldPos = vec3(u_Model * position);
}

#shader fragment
#version 450 core

// output color
layout(location = 0) out vec4 color;

layout(std140, binding = 0) uniform u_Matrices
{
    mat4 cameraProj;
    mat4 cameraView;

    vec4 cameraPos;
    vec4 cameraDir;
};

layout(std140, binding = 1) uniform m_LightingData
{
    vec4 lightPos;
    vec4 lightDir;
    vec4 lightColor;

    float lightIntensity;
    float lightAmbientIntensity;

    float specularStrength;
};

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_UV;

uniform sampler2D u_Tex;

void main()
{
    vec4 tex = texture(u_Tex, v_UV);

    vec3 viewDirection = normalize(cameraPos.xyz - v_WorldPos);

    vec3 lightRay = normalize(vec3(lightDir));
    vec3 reflectedRay = reflect(lightRay, v_Normal);

    // float specularIntensity = pow(max(dot(reflectedRay, -viewDirection), 0.0), specularStrength);

    float diffuseIntensity = min(max(dot(lightRay, v_Normal), 0.0), 1.0) * lightIntensity;

    float intensity = max(diffuseIntensity + lightAmbientIntensity, 0.0);

    vec4 lightRGB = lightColor * intensity;
    color = vec4(tex.x * lightRGB.x, tex.y * lightRGB.y, tex.z * lightRGB.z, tex.w);
}
