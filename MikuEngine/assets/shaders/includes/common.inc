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
