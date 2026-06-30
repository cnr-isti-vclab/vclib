// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHADERS_COMMON_SH
#define VCL_BGFX_SHADERS_COMMON_SH

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#define posToBitFlag(pos) (1u << pos)

// given a buffer composed of uints, each bit represents a boolean, returns the
// value of the bit at the given index (bits are ordered MSB first: bit 31 = index 0)
#define bitSetValueAt(buffer, index) \
    ( (((buffer)[(index) / 32u] & (0x1u << (31u - ((index) % 32u)))) != 0u) )

#define u_normalMatrix mtxFromCols( \
    u_invModelView[0].xyz, u_invModelView[1].xyz, u_invModelView[2].xyz)
// inverse transposed of the model view (without translation)

// bgfx defines front facing as clockwise
// N.B. use this macro only in main functions of shaders
#define vcl_FrontFacing (!gl_FrontFacing)

/**
 * @brief Auxiliary macro to declare a function that fetches a vec3 from a vec4
 * buffer.
 *
 * Buffers cannot be passed as arguments to functions, so we need to declare a
 * function for each buffer that is declared of vec4 but actually contains vec3
 * data.
 * This macro makes easier to declare such functions.
 *
 * @param[in] funcName: The name of the function to be generated.
 * @param[in] bufferName: The name of the vec4 buffer from which the vec3 will
 * be fetched.
 *
 * @code
 * BUFFER_RO(myBuffer, vec4, 0); // buffer of vec4, read only on stage 0
 * DECLARE_FETCH_VEC3(fetchMyBuffer, myBuffer) // declare the function
 * //...
 * vec3 pos = fetchMyBuffer(10); // fetch the 10-th vec3 from myBuffer
 * @endcode
 */
#define DECLARE_FETCH_VEC3(funcName, bufferName) \
    vec3 funcName(uint index) { \
        uint idx30 = index * 3u; \
        uint idx31 = idx30 + 1u; \
        uint idx32 = idx30 + 2u; \
        return vec3( \
            bufferName[idx30 / 4u][idx30 % 4u], \
            bufferName[idx31 / 4u][idx31 % 4u], \
            bufferName[idx32 / 4u][idx32 % 4u]); \
    }

/**
 * @brief Convert an uint color in ABGR format to a vec4 float color.
 * @param[in] color: The input color.
 * @return The output color.
 */
vec4 uintABGRToVec4Color(uint color)
{
    return vec4(
        float(color & uint(0x000000FF)) / 255.0,
        float((color & uint(0x0000FF00)) >> uint(8)) / 255.0,
        float((color & uint(0x00FF0000)) >> uint(16)) / 255.0,
        float((color & uint(0xFF000000)) >> uint(24)) / 255.0);
}

/**
 * @brief Convert an uint color in RGBA format to a vec4 float color.
 * @param[in] color: The input color.
 * @return The output color.
 */
vec4 uintRGBAToVec4Color(uint color) {
    return vec4(
        float((color & uint(0xFF000000)) >> uint(24)) / 255.0,
        float((color & uint(0x00FF0000)) >> uint(16)) / 255.0,
        float((color & uint(0x0000FF00)) >> uint(8)) / 255.0,
        float(color & uint(0x000000FF)) / 255.0);
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

#endif // VCL_BGFX_SHADERS_COMMON_SH
