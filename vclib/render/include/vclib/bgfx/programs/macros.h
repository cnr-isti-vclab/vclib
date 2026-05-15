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

#ifndef VCL_BGFX_PROGRAMS_MACROS_H
#define VCL_BGFX_PROGRAMS_MACROS_H

#define VCLIB_STRINGIFY(x) #x
#define VCLIB_TOSTRING(x) VCLIB_STRINGIFY(x)

// VCLIB_BGFX_SHADER creates the include directive for the shader file.
// usage:
// #include VCLIB_BGFX_SHADER(path, profile, file)
// result:
// #include "path/profile/file"
#define VCLIB_BGFX_SHADER(path, profile, file) \
    VCLIB_TOSTRING(path/profile/file)

// VCLIB_JOIN_WITH_UNDERSCORE concatenates two tokens with an underscore.
// usage:
// VCLIB_JOIN_WITH_UNDERSCORE(token1, token2)
// result:
// token1_token2
#define VCLIB_JOIN_WITH_UNDERSCORE2(a, b) a ## _ ## b
#define VCLIB_JOIN_WITH_UNDERSCORE(a, b) VCLIB_JOIN_WITH_UNDERSCORE2(a, b)

#endif // VCL_BGFX_PROGRAMS_MACROS_H
