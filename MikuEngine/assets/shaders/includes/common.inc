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

vec3 GetForwardFromMatrix( mat4 mtx )
{
	return normalize(vec3(mtx[2]));
}

vec3 GetRightFromMatrix( mat4 mtx )
{
	return normalize(vec3(mtx[0]));
}

vec3 GetUpFromMatrix( mat4 mtx )
{
	return normalize(vec3(mtx[1]));
}

vec4 GetLightIntensity( vec3 v_WorldPos, vec3 v_Normal)
{
    vec3 viewDirection = normalize(cameraPos.xyz - v_WorldPos);

    vec3 lightRay = normalize(vec3(lightDir));
    vec3 reflectedRay = reflect(lightRay, v_Normal);

    float specularIntensity = pow(max(dot(reflectedRay, -viewDirection), 0.0), specularStrength);

    float diffuseIntensity = min(max(dot(lightRay, v_Normal), 0.0), 1.0) * lightIntensity;

    float intensity = max(diffuseIntensity + specularIntensity + lightAmbientIntensity, 0.0);

    return lightColor * intensity;
}
