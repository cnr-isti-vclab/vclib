// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_PBR_COMMON_SH
#define VCL_BGFX_DRAWABLE_PBR_COMMON_SH

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#include <vclib/bgfx/shaders_common.sh>

// Constants

#define GAMMA 2.2

// Settings

// Lighting settings, may not be definitive

#define LIGHT_COUNT                                   1

// use the same lights as defined in other vclib rendering modes (see u_lightDir, u_lightColor)
#define VIEWER_LIGHTS

// precomputed default light directions from https://github.com/KhronosGroup/glTF-Sample-Viewer

#define LIGHT_KEY_DIR       vec3(0.5000000108991332,-0.7071067857071073,-0.49999999460696354)
#define LIGHT_FILL_DIR      vec3(-0.4999998538661192,0.7071068849655084,0.500000052966632)
#define LIGHT_KEY_DIR_VIEW  mul(vec4(0.5000000108991332,-0.7071067857071073,-0.49999999460696354,0.0), u_invView).xyz
#define LIGHT_FILL_DIR_VIEW mul(vec4(-0.4999998538661192,0.7071068849655084,0.500000052966632,0.0), u_invView).xyz

#define DISTRIBUTION_LAMBERTIAN 0u
#define DISTRIBUTION_GGX        1u

#define TONEMAP_NONE                     0
#define TONEMAP_BASIC                    1
#define TONEMAP_ACES_HILL                2
#define TONEMAP_ACES_HILL_EXPOSURE_BOOST 3
#define TONEMAP_ACES_NARKOWICZ           4
#define TONEMAP_KHRONOS_PBR_NEUTRAL      5

/**
 * @brief Applies tone mapping to the given color using the ACES tone mapping modified by Narkowicz.
 * see: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
 * @param[in] color: The color to tone map.
 * @param[in] mapping: The tone mapping operator to use.
 * @return The tone mapped color.
 */
vec3 toneMapACES_Narkowicz(vec3 color)
{
    const float A = 2.51;
    const float B = 0.03;
    const float C = 2.43;
    const float D = 0.59;
    const float E = 0.14;
    return clamp((color * (A * color + B)) / (color * (C * color + D) + E), 0.0, 1.0);
}

vec3 RRTAndODTFit(vec3 color)
{
    vec3 a = color * (color + 0.0245786) - 0.000090537;
    vec3 b = color * (0.983729 * color + 0.4329510) + 0.238081;
    return a / b;
}

/**
 * @brief Applies tone mapping to the given color using the ACES filmic tone map approximation.
 * see https://github.com/TheRealMJP/BakingLab/blob/master/BakingLab/ACES.hlsl
 * @param[in] color: The color to tone map.
 * @return The tone mapped color.
 */
vec3 toneMapACES_Hill(vec3 color)
{

    // sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
    const mat3 ACESInputMat = mat3
    (
        0.59719, 0.07600, 0.02840,
        0.35458, 0.90834, 0.13383,
        0.04823, 0.01566, 0.83777
    );

    // ODT_SAT => XYZ => D60_2_D65 => sRGB
    const mat3 ACESOutputMat = mat3
    (
        1.60475, -0.10208, -0.00327,
        -0.53108,  1.10813, -0.07276,
        -0.07367, -0.00605,  1.07602
    );

    color = mul(color, ACESInputMat);

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = mul(color, ACESOutputMat);

    // Clamp to [0, 1]
    color = clamp(color, 0.0, 1.0);

    return color;
}

/**
 * @brief Applies tone mapping to the given color using Khronos PBR neutral tone mapping.
 * @param[in] color: The color to tone map.
 * @return The tone mapped color.
 */
vec3 toneMap_KhronosPbrNeutral(vec3 color)
{
    const float startCompression = 0.8 - 0.04;
    const float desaturation = 0.15;

    float x = min(color.r, min(color.g, color.b));
    float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
    color -= offset;

    float peak = max(color.r, max(color.g, color.b));
    if (peak < startCompression) return color;

    const float d = 1.0 - startCompression;
    float newPeak = 1.0 - d * d / (peak + d - startCompression);
    color *= newPeak / peak;

    float g = 1.0 - 1.0 / (desaturation * (peak - newPeak) + 1.0);
    return mix(color, newPeak * vec3_splat(1.0), g);
}

/**
 * @brief Applies tone mapping to the given color using the Reinhard operator.
 * @param[in] color: The color to tone map.
 * @return The tone mapped color.
 */
vec3 toneMapBasic(vec3 color)
{
    return color / (color + 1.0);
}

/**
 * @brief Applies tone mapping to the given color using the specified tone mapping operator.
 * @param[in] color: The color to tone map.
 * @param[in] mapping: The tone mapping operator to use.
 * @return The tone mapped color.
 */
vec3 toneMap(vec3 color, int mapping)
{
    switch(mapping)
    {
        case TONEMAP_BASIC:                    return toneMapBasic(color);
        case TONEMAP_ACES_HILL:                return toneMapACES_Hill(color);
        case TONEMAP_ACES_HILL_EXPOSURE_BOOST: return toneMapACES_Hill(color / 0.6); // boost exposure by 0.6
        case TONEMAP_ACES_NARKOWICZ:           return toneMapACES_Narkowicz(color);
        case TONEMAP_KHRONOS_PBR_NEUTRAL:      return toneMap_KhronosPbrNeutral(color);
        default:                               return color;
    }
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
    float denom = NoH2 * (alpha2 - 1.0) + 1.0;
    return alpha2 / (PI * denom * denom);
}

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
 * @brief Applies gamma correction to the given color.
 * @param[in] color: The color to gamma correct.
 * @return The gamma corrected color.
 */
vec3 gammaCorrect(vec3 color)
{
    float oneOverGamma = 1.0 / GAMMA;
    return pow(abs(color), vec3_splat(oneOverGamma));
}

#endif // VCL_BGFX_DRAWABLE_PBR_COMMON_SH
