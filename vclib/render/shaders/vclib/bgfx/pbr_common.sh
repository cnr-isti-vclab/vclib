/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_PBR_COMMON_SH
#define VCL_EXT_BGFX_PBR_COMMON_SH

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#define PBR_VERTEX_COLOR_AVAILABLE 0
#define PBR_ALPHA_MODE_MASK 1

#define USE_LIGHTS
#define LIGHT_COUNT 2
#define PI 3.141592653589793
// precomputed default light directions from https://github.com/KhronosGroup/glTF-Sample-Viewer
#define LIGHT_KEY_DIR vec3(0.5000000108991332,-0.7071067857071073,-0.49999999460696354)
#define LIGHT_FILL_DIR vec3(-0.4999998538661192,0.7071068849655084,0.500000052966632)

/**
 * @brief Computes the dot product of two vectors and clamps it to be >= 0.
 * This is useful for light computations where negative values don't make sense.
 * @param[in] a: The first vector.
 * @param[in] b: The second vector.
 * @return The clamped dot product.
*/
float clampedDot(vec3 a, vec3 b)
{
    return clamp(dot(a, b), 0.0, 1.0);
}

/**
 * @brief BRDF diffuse part (Lambertian).
 * @param[in] color: The fragment color.
 * @return The diffuse light.
 */
vec3 pbrDiffuse(vec3 color)
{
    return color / PI;
}

/**
 * @brief GGX version of the NDF (Normal Distribution Function) which determines the odds for a microfacet normal 
 * to be aligned with the halfway vector H (in other words to reflect light directly).
 * @param[in] NoH: Cosine of the angle between the fragment normal and the halfway vector.
 * @param[in] alpha2: The alpha squared.
 * @return the odds for a microfacet normal to be aligned with the halfway vector H (in other words to reflect light directly).
 */
float D_GGX(
    float NoH,
    float alpha2)
{
    float NoH2 = NoH * NoH;
    float denom = NoH2 * (alpha2 - 1) + 1;
    return alpha2 / (PI * denom * denom);
}

/**
 * @brief GGX version of the Visibility function.
 * The Visibility function or just V determines the odds for a microfacet of not being occluded by some other
 * microfacet. It accounts for both masking and shadowing of microfacets.
 * @param[in] NoV: Cosine of the angle between the fragment normal and the view direction.
 * @param[in] NoL: Cosine of the angle between the fragment normal and the incoming light direction.
 * @param[in] alpha2: The alpha squared.
 * @return The probability that a microfacet is not occluded, accounting for both masking and shadowing effects.
 */
float V_GGX(
    float NoV,
    float NoL,
    float alpha2)
{
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - alpha2) + alpha2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - alpha2) + alpha2);
    float GGX = GGXV + GGXL;
    if(GGX > 0.0)
        return 0.5 / GGX;
    else
        return 0.0;
}

/**
 * @brief Computes the Fresnel factor which tells the amount of light that gets reflected
 * with the Schlick approximation.
 * @param[in] F0: the surface's response at normal incidence (aka base reflectivity),
 *  the amount of light reflected when looking at a surface with a 0 degree angle (right above).
 * @param[in] HoV: Cosine of the angle between the halfway vector H and the view direction V.
 * @return The Fresnel factor.
 */
vec3 F_Schlick(
    vec3 F0,
    vec3 F90,
    float HoV)
{
    return F0 + (F90 - F0) * pow(clamp(1.0 - HoV, 0.0, 1.0), 5.0);
}

/**
 * @brief BRDF specular part.
 * It consists of two major components of which many versions exist:
 *  - The NDF (Normal Distribution Function) or just D determines the odds for a microfacet normal 
 *  to be aligned with the halfway vector H.
 *  - The Visibility function or just V determines the odds for a microfacet of not being occluded by some other
 *  microfacet.
 * @param[in] NoV: Cosine of the angle between the fragment normal and the view direction.
 * @param[in] NoH: Cosine of the angle between the fragment normal and the halfway vector.
 * @param[in] NoL: Cosine of the angle between the fragment normal and the incoming light direction.
 * @param[in] roughness: The roughness of the fragment's material, 
 * ranges from 0 (optically flat) to 1 (very irregular surface that will make reflections more blurry).
 * @return The specular light.
 */
float pbrSpecular(
    float NoV,
    float NoH,
    float NoL,
    float roughness)
{
    // Many versions of D and V remap the roughness parameter.
    // This is done for better looks and/or to make the parameter more easily editable by artists.
    float alpha2 = roughness * roughness * roughness * roughness;
    return V_GGX(NoV, NoL, alpha2) * D_GGX(NoH, alpha2);
}

/**
 * @brief Color computed for Physically Based Rendering (PBR).
 * The incoming light colors are altered by:
 *  the Cook-Torrance BRDF (Bidirectional Reflective Distribution Function)
 *  which depends on the material properties of the lit fragment (right now see params: color, metallic, roughness).
 * and
 *  the cosine of the angle between the fragment's normal and the light's direction;
 *  the cosine is given as the dot product of the two.
 *
 * The Cook-Torrance BRDF consists of two parts a diffuse and a specular one:
 *  Specular: light that gets reflected immediately after contact with the surface.
 *  Diffuse: escaped light that got refracted.
 * the Fresnel factor ks tells how much of the incoming light gets reflected so it can work as a weight
 * for the two parts with kd = 1 - ks (so that kd + ks = 1) for energy conservation.
 *
 * @param[in] vPos: The fragment position.
 * @param[in] cameraEyePos: The camera position.
 * @param[in] lightDirs: The incoming light directions, must be normalized.
 * @param[in] lightColors: The incoming light colors (RGB) (their radiance approximated).
 * @param[in] lightIntensities: The incoming light intensities (scalars).
 * @param[in] color: The fragment color or albedo (RGB).
 * @param[in] normal: The fragment normal, must be normalized.
 * @param[in] metallic: The metalness of the fragment's material, ranges from 0 (dielectric) to 1 (metal). 
 * @param[in] roughness: The roughness of the fragment's material, ranges from 0 (optically flat) to 1 (very irregular surface).
 * @param[in] emissive: The emissive color (RGB) of the fragment's material.
 * @return The color (RGB) reflected by the fragment, tone mapped and gamma corrected.
 */
vec4 pbrColor(
    vec3 vPos,
    vec3 cameraEyePos,
    vec3 lightDirs[LIGHT_COUNT],
    vec3 lightColors[LIGHT_COUNT],
    float lightIntensities[LIGHT_COUNT],
    vec4 color,
    vec3 normal,
    float metallic,
    float roughness,
    vec3 emissive)
{
    vec3 finalColor = vec3(0.0,0.0,0.0);
    vec3 f0_dielectric = vec3(0.04, 0.04, 0.04);
    vec3 f90 = vec3(1.0, 1.0, 1.0);

    #ifdef USE_LIGHTS

    // view direction
    vec3 V = normalize(cameraEyePos - vPos);

    if (dot(normal, V) < 0.0) 
        normal = -normal;
    
    float NoV = clampedDot(normal, V);

    for(int i = 0; i < LIGHT_COUNT; ++i)
    {
        // incoming light direction and contribution
        vec3 lightDir = normalize(-lightDirs[i]);
        float NoL = clampedDot(normal, lightDir);
        vec3 lightIntensity = lightIntensities[i] * lightColors[i] * NoL;

        // halfway vector, same angle with both view direction and incoming light direction
        // corresponds to the normal that one microfacet must have to directly reflect the light into the eye
        vec3 H = normalize(V + lightDir);
        // related dot products
        float NoH = clampedDot(normal, H);
        float HoV = clampedDot(H, V);

        // Fresnel factors for both dielectric and metallic surfaces
        // 0.04 is an approximation of F0 averaged around many dielectric materials
        vec3 dielectric_fresnel = F_Schlick(f0_dielectric, f90, HoV);
        // Metals have the surface color as base reflectivity since no light gets absorbed
        vec3 metal_fresnel = F_Schlick(color.rgb, f90, HoV);

        // diffuse component
        vec3 l_diffuse = lightIntensity * pbrDiffuse(color.rgb);

        // specular component for both metallic and dielectric surfaces
        vec3 l_specular_metal = lightIntensity * pbrSpecular(NoV, NoH, NoL, roughness);
        vec3 l_specular_dielectric = l_specular_metal;

        // metallic surfaces reflect only specular light
        vec3 l_metal_brdf = metal_fresnel * l_specular_metal;

        // dielectric surfaces reflect both diffuse and specular light
        vec3 l_dielectric_brdf = mix(l_diffuse, l_specular_dielectric, dielectric_fresnel);

        // final color is a mix of both dielectric and metallic BRDFs based on the metalness of the surface
        // the interpolation is needed as we consider the metallic value as ranged instead of binary
        vec3 l_color = mix(l_dielectric_brdf, l_metal_brdf, metallic);

        finalColor += l_color;
    }
    #endif // USE_LIGHTS

    // add emissive component
    finalColor += emissive;

    // tone mapping - Reinhard operator
    //finalColor = finalColor / (finalColor + vec3(1.0, 1.0, 1.0));

    // gamma correction
    float oneOverGamma = 1.0 / 2.2;
    finalColor = pow(abs(finalColor), vec3(oneOverGamma, oneOverGamma, oneOverGamma));

    return vec4(finalColor.r, finalColor.g, finalColor.b, color.a);
}
#endif // VCL_EXT_BGFX_PBR_COMMON_SH
