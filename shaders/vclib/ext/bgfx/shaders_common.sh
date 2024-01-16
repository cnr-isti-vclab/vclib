#ifndef VCL_EXT_BGFX_SHADERS_COMMON_SH
#define VCL_EXT_BGFX_SHADERS_COMMON_SH

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

vec4 uintToVec4Color(uint color)
{
    return vec4(
        float(color & uint(0x000000FF)) / 255.0,
        float((color & uint(0x0000FF00)) >> uint(8)) / 255.0,
        float((color & uint(0x00FF0000)) >> uint(16)) / 255.0,
        float((color & uint(0xFF000000)) >> uint(24)) / 255.0);
}

vec4 computeLight(
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal,
    float ambientStrength = 0.2)
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
    // diffuse
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    return vec4(ambient + diffuse, 1.0);
}

vec3 computeSpecular(
    vec3 vPos,
    vec3 cameraEyePos,
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal,
    float specularStrength = 0.3)
{
    vec3 viewDir = normalize(cameraEyePos - vPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;
}

#endif // VCL_EXT_BGFX_SHADERS_COMMON_SH
