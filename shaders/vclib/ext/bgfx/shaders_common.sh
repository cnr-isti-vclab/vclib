/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

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
    float ambientStrength)
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
    // diffuse
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    return vec4(ambient + diffuse, 1.0);
}

vec4 computeLight(
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal)
{
    return computeLight(lightDir, lightColor, normal, 0.2);
}

vec3 computeSpecular(
    vec3 vPos,
    vec3 cameraEyePos,
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal,
    float specularStrength)
{
    vec3 viewDir = normalize(cameraEyePos - vPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;
}

vec3 computeSpecular(
    vec3 vPos,
    vec3 cameraEyePos,
    vec3 lightDir,
    vec3 lightColor,
    vec3 normal)
{
    return computeSpecular(
        vPos, cameraEyePos, lightDir, lightColor, normal, 0.3);
}

#endif // VCL_EXT_BGFX_SHADERS_COMMON_SH
