// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_ENVIRONMENT_UNIFORMS_SH
#define VCL_BGFX_DRAWABLE_DRAWABLE_ENVIRONMENT_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/pbr_common.sh>

uniform vec4 u_environmentSettingsPack;

#define u_roughness u_environmentSettingsPack.x
#define u_cubeSideResolution u_environmentSettingsPack.y

/**
 * @brief Computes the solid angle covered by the rectangle starting from (0,0) to some given (u,v) projected onto a unit sphere.
 * Presumably used for cubemap texel solid angle computation.
 * @param[in] uv: The UV coordinates.
 * @return The solid angle covered by the rectangle starting from (0,0) to some given (u,v).
 */
float solidAngle00ToUv(vec2 uv)
{
    return atan2(uv.x * uv.y, sqrt(uv.x * uv.x + uv.y * uv.y + 1.0));
}

/**
 * @brief Computes the solid angle covered by a texel in UV space projected onto a unit sphere.
 * Presumably used for cubemap texel solid angle computation.
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
    MicrofacetDistributionSample smp;

    if(distributionType == DISTRIBUTION_LAMBERTIAN)
    {
        smp = Lambertian(Xi);
    }
    else // if(distributionType == DISTRIBUTION_GGX)
    {
        smp = GGX(Xi, roughness);
    }

    // from spherical coordinates to cartesian coordinates
    vec3 H = normalize(vec3(
        cos(smp.phi) * smp.sinTheta,
        sin(smp.phi) * smp.sinTheta,
        smp.cosTheta
    ));

    // from tangent-space vector to world-space sample vector
    mat3 TBN = generateTBN(N);
    vec3 sampleVec = mul(H, TBN);

    return vec4(sampleVec.x, sampleVec.y, sampleVec.z, smp.pdf);
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
 * @brief Computes the direction vector for a given cubemap face and UV coordinates.
 * @param[in] face: The cubemap face index (0-5).
 * @param[in] uv: The UV coordinates in the range [-1,1].
 * @param[in] fromHdr: Whether the cubemap is from an HDR source (flipped for writing the cubemap).
 * @return The direction vector corresponding to the given face and UV coordinates.
 */
vec3 faceDirection(uint face, vec2 uv, bool fromHdr)
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

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_ENVIRONMENT_UNIFORMS_SH
