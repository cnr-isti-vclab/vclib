/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/bgfx/drawable/mesh/pbr_macros.h>

// Constants

#define PI                                            3.141592653589793
#define GAMMA                                         2.2

// Settings

#define checkSetting(settings, setting) bool(settings & posToBitFlag(setting))

#define isPerVertexColorAvailable(settings)   checkSetting(settings, VCL_PBR_VERTEX_COLOR)
#define isPerVertexTangentAvailable(settings) checkSetting(settings, VCL_PBR_VERTEX_TANGENT)
#define isAlphaModeMask(settings)             checkSetting(settings, VCL_PBR_IS_ALPHA_MODE_MASK)
#define useImageBasedLighting(settings)       checkSetting(settings, VCL_PBR_IMAGE_BASED_LIGHTING)

#define isBaseColorTextureAvailable(settings)         checkSetting(settings, VCL_PBR_TEXTURE_BASE_COLOR)
#define isMetallicRoughnessTextureAvailable(settings) checkSetting(settings, VCL_PBR_TEXTURE_METALLIC_ROUGHNESS)
#define isNormalTextureAvailable(settings)            checkSetting(settings, VCL_PBR_TEXTURE_NORMAL)
#define isOcclusionTextureAvailable(settings)         checkSetting(settings, VCL_PBR_TEXTURE_OCCLUSION)
#define isEmissiveTextureAvailable(settings)          checkSetting(settings, VCL_PBR_TEXTURE_EMISSIVE)

// Lighting settings, may not be definitive

#define LIGHT_COUNT                                   1

// use the same lights as defined in other vclib rendering modes (see u_lightDir, u_lightColor)
#define VIEWER_LIGHTS

// precomputed default light directions from https://github.com/KhronosGroup/glTF-Sample-Viewer

#define LIGHT_KEY_DIR                                 vec3(0.5000000108991332,-0.7071067857071073,-0.49999999460696354)
#define LIGHT_FILL_DIR                                vec3(-0.4999998538661192,0.7071068849655084,0.500000052966632)
#define LIGHT_KEY_DIR_VIEW                            mul(vec4(0.5000000108991332,-0.7071067857071073,-0.49999999460696354,0.0), u_invView).xyz
#define LIGHT_FILL_DIR_VIEW                           mul(vec4(-0.4999998538661192,0.7071068849655084,0.500000052966632,0.0), u_invView).xyz

#define DISTRIBUTION_LAMBERTIAN                     0u
#define DISTRIBUTION_GGX                            1u

#define TONEMAP_NONE                     0
#define TONEMAP_BASIC                    1
#define TONEMAP_ACES_HILL                2
#define TONEMAP_ACES_HILL_EXPOSURE_BOOST 3
#define TONEMAP_ACES_NARKOWICZ           4
#define TONEMAP_KHRONOS_PBR_NEUTRAL      5

/**
 * @brief Computes the solid angle covered by the rectangle starting from (0,0) to some given (u,v) projected onto a unit sphere.
 * Supposedly used for cubemap texel solid angle computation. 
 * @param[in] uv: The UV coordinates.
 * @return The solid angle covered by the rectangle starting from (0,0) to some given (u,v).
 */
float solidAngle00ToUv(vec2 uv)
{
    return atan2(uv.x * uv.y, sqrt(uv.x * uv.x + uv.y * uv.y + 1.0));
}

/**
 * @brief Computes the solid angle covered by a texel in UV space projected onto a unit sphere.
 * Supposedly used for cubemap texel solid angle computation. 
 * @param[in] uv: The top-left UV coordinates of the texel.
 * @param[in] invSize: The inverted size of the cube needed to compute the other angle coordinates of the texel.
 * @return The solid angle covered by the texel.
 */
float solidAngle(vec2 uv, float invSize)
{
    vec2 A = vec2(uv.x,           uv.y + invSize);
    vec2 B = vec2(uv.x + invSize, uv.y + invSize);
    vec2 C = vec2(uv.x + invSize, uv.y          );
    vec2 D = vec2(uv.x          , uv.y          );
    return 
        solidAngle00ToUv(A) -
        solidAngle00ToUv(B) +
        solidAngle00ToUv(C) -
        solidAngle00ToUv(D);
}

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
 * @brief Computes the radical inverse of a number using the Van der Corput sequence.
 * @param[in] bits: The input number.
 * @return The radical inverse of the input number.
 */
float radicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

/**
 * @brief Computes the Hammersley point set for a given index and total number of samples.
 * @param[in] i: The sample index.
 * @param[in] N: The total number of samples.
 * @return The Hammersley point set.
 */
vec2 hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}  

/**
 * @brief Generates a Tangent-Bitangent-Normal (TBN) matrix given a normal vector.
 * @param[in] normal: The normal vector (must be normalized).
 * @return The TBN matrix.
 */
mat3 generateTBN(vec3 normal)
{
    vec3 bitangent = vec3(0.0, 1.0, 0.0);

    float NdotUp = dot(normal, vec3(0.0, 1.0, 0.0));
    float epsilon = 0.0000001;
    if (1.0 - abs(NdotUp) <= epsilon)
    {
        // Sampling +Y or -Y, so we need a more robust bitangent.
        if (NdotUp > 0.0)
        {
            bitangent = vec3(0.0, 0.0, 1.0);
        }
        else
        {
            bitangent = vec3(0.0, 0.0, -1.0);
        }
    }

    vec3 tangent = normalize(cross(bitangent, normal));
    bitangent = cross(normal, tangent);

    return mat3(tangent, bitangent, normal);
}

/**
 * @brief Structure representing a sample from a microfacet distribution.
 */
struct MicrofacetDistributionSample
{
    float pdf;
    float cosTheta;
    float sinTheta;
    float phi;
};

/**
 * @brief Generates a sample from the hemisphere following the Lambertian distribution weighted by the cosine of the angle.
 * @param[in] xi: The random sample in [0,1]^2.
 * @return The Lambertian microfacet distribution sample.
 */
MicrofacetDistributionSample Lambertian(vec2 xi)
{
    MicrofacetDistributionSample lambertian;

    // Cosine weighted hemisphere sampling
    // http://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations.html#Cosine-WeightedHemisphereSampling
    lambertian.cosTheta = sqrt(1.0 - xi.y);
    lambertian.sinTheta = sqrt(xi.y);
    lambertian.phi = 2.0 * PI * xi.x;

    // evaluation for solid angle, therefore drop the sinTheta
    lambertian.pdf = lambertian.cosTheta / PI;

    return lambertian;
}

/**
 * @brief Generates a sample from the hemisphere following the GGX microfacet distribution.
 * @param[in] xi: The random sample in [0,1]^2.
 * @param[in] roughness: The surface roughness.
 * @return The GGX microfacet distribution sample.
 */
MicrofacetDistributionSample GGX(vec2 xi, float roughness)
{
    MicrofacetDistributionSample ggx;

    // GGX microfacet distribution
    // https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.html
    // This implementation is based on https://bruop.github.io/ibl/,
    //  https://www.tobias-franke.eu/log/2014/03/30/notes_on_importance_sampling.html
    // and https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch20.html

    // evaluate sampling equations
    float alpha = roughness * roughness;
    ggx.cosTheta = saturate(sqrt((1.0 - xi.y) / (1.0 + (alpha * alpha - 1.0) * xi.y)));
    ggx.sinTheta = sqrt(1.0 - ggx.cosTheta * ggx.cosTheta);
    ggx.phi = 2.0 * PI * xi.x;

    // evaluate GGX pdf (for half vector)
    // Apply the Jacobian to obtain a pdf that is parameterized by l
    // Typically you'd have the following:
    // float pdf = D_GGX(NoH, roughness) * NoH / (4.0 * VoH);
    // but since V = N => VoH == NoH
    ggx.pdf = D_GGX(ggx.cosTheta, alpha * alpha) / 4.0;

    return ggx;
}

/**
 * @brief Generates a sample vector from the hemisphere according to the specified microfacet distribution.
 * @param[in] sampleIndex: The index of the sample.
 * @param[in] sampleCount: The total number of samples.
 * @param[in] N: The normal vector.
 * @param[in] distributionType: The type of microfacet distribution to use.
 * @param[in] roughness: The surface roughness (used only for the distributions which require it).
 * @return The sampled vector and its pdf.
 */
vec4 getImportanceSample(uint sampleIndex, uint sampleCount, vec3 N, uint distributionType, float roughness)
{
    vec2 Xi = hammersley(sampleIndex, sampleCount);
    MicrofacetDistributionSample sample;

    if(distributionType == DISTRIBUTION_LAMBERTIAN)
    {
        sample = Lambertian(Xi);
    }
    else // if(distributionType == DISTRIBUTION_GGX)
    {
        sample = GGX(Xi, roughness);
    }

    // from spherical coordinates to cartesian coordinates
    vec3 H = normalize(vec3(
        cos(sample.phi) * sample.sinTheta,
        sin(sample.phi) * sample.sinTheta,
        sample.cosTheta
    ));

    // from tangent-space vector to world-space sample vector
    mat3 TBN = generateTBN(N);
    vec3 sampleVec = mul(H, TBN);

    return vec4(sampleVec.x, sampleVec.y, sampleVec.z, sample.pdf);
}

/**
 * @brief Computes the appropriate mip level to sample from a cubemap given the PDF of the sample,
 * the cubemap width and the number of samples taken.
 * Approximation used to reduce the number of texture lookups.
 * See: https://developer.nvidia.com/gpugems/gpugems3/part-iii-rendering/chapter-20-gpu-based-importance-sampling
 * and  https://cgg.mff.cuni.cz/~jaroslav/papers/2007-sketch-fis/Final_sap_0073.pdf
 * @param[in] pdf: The probability density function value of the sample.
 * @param[in] width: The width of the cubemap.
 * @param[in] sampleCount: The number of samples taken.
 * @return The appropriate mip level to sample from.
 */
float computeLod(float pdf, float width, float sampleCount)
{
    // Solid angle of current sample -- bigger for less likely samples

    // float omegaS = 1.0 / (sampleCount * pdf);

    // Solid angle of texel

    // float omegaP = 1.0 / (6.0 * float(u_width) * float(u_width));

    // Mip level is determined by the ratio of our sample's solid angle to a texel's solid angle 
    // note that 0.5 * log2 is equivalent to log4

    // float lod = 0.5 * log2(omegaS / omegaP);

    return 0.5 * log2( (6.0 * width * width) / (sampleCount * pdf));
}

/**
 * @brief Converts a right-handed coordinate system vector to a left-handed coordinate system vector.
 * @param[in] rightHand: The right-handed coordinate system vector.
 * @return The left-handed coordinate system vector.
 */
vec3 leftHand(vec3 rightHand)
{
    return vec3(rightHand.x, rightHand.y, -rightHand.z);
}

/**
 * @brief Computes the direction vector for a given cubemap face and UV coordinates.
 * @param[in] face: The cubemap face index (0-5).
 * @param[in] uv: The UV coordinates in the range [-1,1].
 * @param[in] fromHdr: Whether the cubemap is from an HDR source (flipped for writing the cubemap).
 * @return The direction vector corresponding to the given face and UV coordinates.
 */
vec3 faceDirection(int face, vec2 uv, bool fromHdr)
{
    if(fromHdr) // flipped for writing
    {
        switch(face)
        {
            case 0: return normalize(vec3(-uv.x, uv.y,   1.0));
            case 1: return normalize(vec3( uv.x, uv.y,  -1.0));
            case 2: return normalize(vec3( uv.y, -1.0,  uv.x));
            case 3: return normalize(vec3(-uv.y,  1.0,  uv.x));
            case 4: return normalize(vec3(  1.0, uv.y,  uv.x));
            case 5: return normalize(vec3( -1.0, uv.y, -uv.x));
            default: return vec3_splat(0.0);
        }
    }
    else switch(face)
    {
        case 0: return normalize(vec3(  1.0, -uv.y,  uv.x));
        case 1: return normalize(vec3( -1.0, -uv.y, -uv.x));
        case 2: return normalize(vec3( uv.x,   1.0, -uv.y));
        case 3: return normalize(vec3( uv.x,  -1.0,  uv.y));
        case 4: return normalize(vec3( uv.x, -uv.y,  -1.0));
        case 5: return normalize(vec3(-uv.x, -uv.y,   1.0));
        default: return vec3_splat(0.0);
    }
}

/**
 * @brief Converts a direction vector to equirectangular UV coordinates.
 * @param[in] dir: The direction vector.
 * @return The equirectangular UV coordinates.
 */
vec2 dirToEquirectUV(vec3 dir)
{
   float phi   = atan2(dir.z, dir.x);       // [-pi..pi]
   float theta = asin(dir.y);               // [-pi/2..pi/2]

   float u = (phi   / (2.0*PI)) + 0.5;
   float v = (theta / PI) + 0.5;

   return vec2(u, v);
}

#if BGFX_SHADER_TYPE_FRAGMENT

/**
 * @brief Computes the tangent frame (Tangent, Bitangent, Normal) matrix given its vectors.
 * @param[in] tangent: The fragment tangent vector.
 * @param[in] bitangent: The fragment bitangent vector.
 * @param[in] normal: The fragment normal vector.
 * @param[in] frontFacing: Tells if the fragment is facing the front of the camera.
 *  In case the fragment is not front facing, the frame vectors will be negated.
 * @return The tangent frame matrix.
 */
mat3 tangentFrameFromGivenVectors(vec3 tangent, vec3 bitangent, vec3 normal, bool frontFacing)
{
    if(!frontFacing)
    {
        tangent *= -1.0;
        bitangent *= -1.0;
        normal *= -1.0;
    }

    return mat3(
        normalize(tangent),
        normalize(bitangent),
        normalize(normal)
    );
}

/**
 * @brief Computes the tangent frame (Tangent, Bitangent, Normal) matrix given the normal, position and UV.
 * The tangent and bitangent vectors are computed using the derivatives of position and UV.
 * @param[in] normal: The fragment normal vector.
 * @param[in] position: The fragment position.
 * @param[in] UV: The fragment UV coordinates.
 * @param[in] frontFacing: Tells if the fragment is facing the front of the camera.
 *  In case the fragment is not front facing, the frame vectors will be negated.
 * @return The tangent frame matrix.
 */
mat3 tangentFrameFromNormal(vec3 normal, vec3 position, vec2 UV, bool frontFacing)
{
    // see https://learnopengl.com/Advanced-Lighting/Normal-Mapping

    // get UV derivatives
    vec2 uv_dx = dFdx(UV); // = vec2(du1, dv1)
    vec2 uv_dy = dFdy(UV); // = vec2(du2, dv2)

    /* Present in gltf sample renderer but may be detrimental depending on how the UV derivatives are computed.
        if (length(uv_dx) <= 1e-2)
            uv_dx = vec2(1.0, 0.0);

        if (length(uv_dy) <= 1e-2)
            uv_dy = vec2(0.0, 1.0);
    */

    // compute tangent based on the fact that the edges of the triangle can be expressed as:
    // e1 = du1 * t + dv1 * b
    // e2 = du2 * t + dv2 * b
    // and can be actually computed as:
    // e1 = dFdx(position)
    // e2 = dFdy(position)
    // we can solve the linear system to find just t:
    vec3 t_ =
        (uv_dy.y * dFdx(position) - uv_dx.y * dFdy(position)) /
        (uv_dx.x * uv_dy.y - uv_dy.x * uv_dx.y);

    vec3 n = normalize(normal);
    // computed tangent t_ may not be orthogonal to normal, make it so
    vec3 t = normalize(t_ - n * dot(n, t_));
    // compute bitangent as cross product (orthongonal to both normal and tangent)
    vec3 b = cross(n, t);

    if(!frontFacing)
    {
        t *= -1.0;
        b *= -1.0;
        n *= -1.0;
    }

    return mat3(t, b, n);
}

/**
 * @brief Computes the tangent frame (Tangent, Bitangent, Normal) matrix given the position and UV.
 * The normal, tangent and bitangent vectors are computed using the derivatives of position and UV.
 * @param[in] position: The fragment position.
 * @param[in] UV: The fragment UV coordinates.
 * @param[in] frontFacing: Tells if the fragment is facing the front of the camera.
 *  In case the fragment is not front facing, the frame vectors will be negated.
 * @return The tangent frame matrix.
 */
mat3 tangentFrameFromPosition(vec3 position, vec2 UV, bool frontFacing)
{
    // if even the normal is not provided
    // compute it as orthogonal to the surface defined by position derivatives (two triangle edges)
    vec3 normal = cross(dFdx(position), dFdy(position));
    return tangentFrameFromNormal(normal, position, UV, frontFacing); // will normalize the normal
}

#endif

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
 * @param[in] F90: the surface's response at grazing angles (90 degrees),
 *  the amount of light reflected when looking at a surface with a 90 degree angle (from the side).
 * @param[in] VoH: Cosine of the angle between the halfway vector H and the view direction V.
 * @return The Fresnel factor.
 */
vec3 F_Schlick(
    vec3 F0,
    vec3 F90,
    float VoH)
{
    return F0 + (F90 - F0) * pow(clamp(1.0 - VoH, 0.0, 1.0), 5.0);
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
 * @brief Computes the Image Based Lighting (IBL) Fresnel term using the GGX specular BRDF.
 * It accounts for both single and multiple scattering.
 * @param[in] brdf: The precomputed BRDF lookup values.
 * @param[in] NoV: Cosine of the angle between the fragment normal and the view direction.
 * @param[in] roughness: The roughness of the fragment's material, 
 * ranges from 0 (optically flat) to 1 (very irregular surface that will make reflections more blurry).
 * @param[in] F0: the surface's response at normal incidence (aka base reflectivity),
 *  the amount of light reflected when looking at a surface with a 0 degree angle (right above).
 * @return The IBL Fresnel term.
 */
vec3 iblGgxFresnel(vec2 brdf, float NoV, float roughness, vec3 F0)
{
    float specularWeight = 1.0; // related to some extension, for now use a neutral value
    // see https://bruop.github.io/ibl/#single_scattering_results at Single Scattering Results
    // Roughness dependent fresnel, from Fdez-Aguera
    vec3 Fr = max(vec3_splat(1.0 - roughness), F0) - F0;
    vec3 k_S = F0 + Fr * pow(1.0 - NoV, 5.0);
    vec3 FssEss = specularWeight * (k_S * brdf.x + brdf.y);

    // Multiple scattering, from Fdez-Aguera
    float Ems = (1.0 - (brdf.x + brdf.y));
    vec3 F_avg = specularWeight * (F0 + (1.0 - F0) / 21.0);
    vec3 FmsEms = Ems * FssEss * F_avg / (1.0 - F_avg * Ems);

    return FssEss + FmsEms;
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

/**
 * @brief Color computed for Physically Based Rendering (PBR).
 * The incoming light colors are altered by:
 *  the Cook-Torrance BRDF (Bidirectional Reflective Distribution Function)
 *  which depends on the material properties of the lit fragment.
 * and
 *  the cosine of the angle between the fragment's normal and the light's direction;
 *  the cosine is given as the dot product of the two.
 *
 * The Cook-Torrance BRDF consists of two parts a diffuse and a specular one:
 *  Specular: light that gets reflected immediately after contact with the surface.
 *  Diffuse: escaped light that got refracted.
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
 * @param[in] exposure: The exposure factor.
 * @param[in] toneMapping: The tone mapping operator to use.
 * @return The color (RGB) reflected by the fragment, tone mapped and gamma corrected.
 */
vec4 pbrColorLights(
    vec3 vPos,
    vec3 cameraEyePos,
    vec3 lightDirs[LIGHT_COUNT],
    vec3 lightColors[LIGHT_COUNT],
    float lightIntensities[LIGHT_COUNT],
    vec4 color,
    vec3 normal,
    float metallic,
    float roughness,
    vec3 emissive,
    float exposure,
    int toneMapping)
{
    vec3 finalColor = vec3_splat(0.0);
    vec3 f0_dielectric = vec3_splat(0.04);
    vec3 f90 = vec3_splat(1.0);

    // view direction
    vec3 V = normalize(cameraEyePos - vPos);
    
    float NoV = clampedDot(normal, V);

    UNROLL
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
        float VoH = clampedDot(V, H);

        // Fresnel factors for both dielectric and metallic surfaces
        // 0.04 is an approximation of F0 averaged around many dielectric materials
        vec3 dielectric_fresnel = F_Schlick(f0_dielectric, f90, abs(VoH));
        // Metals have the surface color as base reflectivity since no light gets absorbed
        vec3 metal_fresnel = F_Schlick(color.rgb, f90, abs(VoH));

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

    // add emissive component
    finalColor += emissive;

    finalColor *= exposure;

    // tone mapping 
    finalColor = toneMap(finalColor, toneMapping);

    // gamma correction
    finalColor = gammaCorrect(finalColor);

    return vec4(finalColor.r, finalColor.g, finalColor.b, color.a);
}

/**
 * @brief Computes the color for Physically Based Rendering (PBR) using Image Based Lighting (IBL).
 * The incoming light colors are altered by:
 *  the Cook-Torrance BRDF (Bidirectional Reflective Distribution Function)
 *  which depends on the material properties of the lit fragment.
 * and
 *  the cosine of the angle between the fragment's normal and the light's direction;
 *  the cosine is given as the dot product of the two.
 *
 * The Cook-Torrance BRDF consists of two parts a diffuse and a specular one:
 *  Specular: light that gets reflected immediately after contact with the surface.
 *  Diffuse: escaped light that got refracted.
 *
 * @param[in] diffuseLight: The incoming diffuse light color (RGB).
 * @param[in] color: The fragment color or albedo (RGBA).
 * @param[in] radiance: The incoming specular light color (RGB).
 * @param[in] metalFresnel: The Fresnel factor for metallic surfaces (RGB).
 * @param[in] dielectricFresnel: The Fresnel factor for dielectric surfaces (RGB).
 * @param[in] metallic: The metalness of the fragment's material, ranges from 0 (dielectric) to 1 (metal). 
 * @param[in] occlusion: The ambient occlusion factor, ranges from 0 (fully occluded) to 1 (not occluded).
 * @param[in] emissive: The emissive color (RGB) of the fragment's material.
 * @param[in] exposure: The exposure factor.
 * @param[in] toneMapping: The tone mapping operator to use.
 * @return The color (RGB) reflected by the fragment, tone mapped and gamma corrected.
 */
vec4 pbrColorIbl(
    vec3 diffuseLight,
    vec4 color,
    vec3 radiance,
    vec3 metalFresnel,
    vec3 dielectricFresnel,
    float metallic,
    float occlusion,
    vec3 emissive,
    float exposure,
    int toneMapping)
{
    vec3 finalColor = vec3_splat(0.0);

    vec3 f_diffuse = diffuseLight * color.rgb;

    vec3 f_specular_metal = radiance;
    vec3 f_specular_dielectric = f_specular_metal;

    vec3 f_metal_brdf_ibl = metalFresnel * f_specular_metal;
 
    vec3 f_dielectric_brdf_ibl = mix(f_diffuse, f_specular_dielectric, dielectricFresnel);

    finalColor = mix(f_dielectric_brdf_ibl, f_metal_brdf_ibl, metallic);

    finalColor *= occlusion;

    finalColor += emissive;

    finalColor *= exposure;

    finalColor = toneMap(finalColor, toneMapping);

    finalColor = gammaCorrect(finalColor);

    return vec4(finalColor.r, finalColor.g, finalColor.b, color.a);
}


#endif // VCL_EXT_BGFX_PBR_COMMON_SH
