// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_SURFACE_PBR_COMMON_SH
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_SURFACE_PBR_COMMON_SH

#include <vclib/bgfx/drawable/pbr_common.sh>

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
 * @brief BRDF diffuse part (Lambertian).
 * @param[in] color: The fragment color.
 * @return The diffuse light.
 */
vec3 pbrDiffuse(vec3 color)
{
    return color / PI;
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
vec3 iblGgxFresnel(vec2 brdf, float NoV, float roughness, vec3 F0, float specularWeight)
{
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
    float specular,
    vec3 specularColor,
    float exposure,
    int toneMapping)
{
    vec3 finalColor = vec3_splat(0.0);
    vec3 f0_dielectric = min(vec3_splat(0.04) * specularColor, vec3_splat(1.0));
    vec3 f90 = vec3_splat(1.0);
    vec3 f90_dielectric = vec3_splat(specular);

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
        vec3 dielectric_fresnel = F_Schlick(f0_dielectric * specular, f90_dielectric, abs(VoH));
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

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_SURFACE_PBR_COMMON_SH
