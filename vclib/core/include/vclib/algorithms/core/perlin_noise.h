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

#ifndef VCL_ALGORITHMS_CORE_PERLIN_NOISE_H
#define VCL_ALGORITHMS_CORE_PERLIN_NOISE_H

#include <cmath>

// TODO: Improve the documentation of these functions.

namespace vcl {

namespace detail {

inline int perlinNoiseP(int i)
{
    static int p[512] = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
        7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,
        23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252,
        219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,
        174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,
        27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230,
        220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,
        63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
        200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
        3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
        255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
        28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,
        221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228,
        251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
        145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
        184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236,
        205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
        215, 61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,
        96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,
        37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
        197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
        71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
        60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102,
        143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187,
        208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100,
        109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202,
        38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
        58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
        44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,
        39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104,
        218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179,
        162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
        181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141,
        128, 195, 78,  66,  215, 61,  156, 180};
    return p[i];
}

inline double perlinNoiseGrad(int hash, double x, double y, double z)
{
    int    h = hash & 15;     // CONVERT LO 4 BITS OF HASH CODE
    double u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
        v    = h < 4              ? y :
               h == 12 || h == 14 ? x :
                                    z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

inline double perlinNoiseLerp(double t, double a, double b)
{
    return a + t * (b - a);
}

inline double perlinNoiseFade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

} // namespace detail

/**
 * @brief 3D Perlin noise, returns a value in the [-1,1] range with period 255.
 *
 * @internal This implementation is based on the java reference implementation
 * published on http://mrl.nyu.edu/~perlin/noise/.
 *
 * @param x
 * @param y
 * @param z
 * @return
 *
 * @ingroup space_core
 */
inline double perlinNoise(double x, double y, double z)
{
    int X = (int) floor(x) & 255, // FIND UNIT CUBE THAT
        Y = (int) floor(y) & 255, // CONTAINS POINT.
        Z = (int) floor(z) & 255;
    x -= floor(x); // FIND RELATIVE X,Y,Z
    y -= floor(y); // OF POINT IN CUBE.
    z -= floor(z);
    double u = detail::perlinNoiseFade(x), // COMPUTE FADE CURVES
        v    = detail::perlinNoiseFade(y), // FOR EACH OF X,Y,Z.
        w    = detail::perlinNoiseFade(z);
    int A = detail::perlinNoiseP(X) + Y, AA = detail::perlinNoiseP(A) + Z,
        AB = detail::perlinNoiseP(A + 1) + Z, // HASH COORDINATES OF
        B = detail::perlinNoiseP(X + 1) + Y, BA = detail::perlinNoiseP(B) + Z,
        BB = detail::perlinNoiseP(B + 1) + Z; // THE 8 CUBE CORNERS,

    return detail::perlinNoiseLerp(
        w,
        detail::perlinNoiseLerp(
            v,
            detail::perlinNoiseLerp(
                u,
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(AA), x, y, z), // AND ADD
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(BA), x - 1, y, z)), // BLENDED
            detail::perlinNoiseLerp(
                u,
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(AB), x, y - 1, z), // RESULTS
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(BB), x - 1, y - 1, z))), // FROM  8
        detail::perlinNoiseLerp(
            v,
            detail::perlinNoiseLerp(
                u,
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(AA + 1), x, y, z - 1), // CORNERS
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(BA + 1), x - 1, y, z - 1)), // OF
                                                                     // CUBE
            detail::perlinNoiseLerp(
                u,
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(AB + 1), x, y - 1, z - 1),
                detail::perlinNoiseGrad(
                    detail::perlinNoiseP(BB + 1), x - 1, y - 1, z - 1))));
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_PERLIN_NOISE_H
