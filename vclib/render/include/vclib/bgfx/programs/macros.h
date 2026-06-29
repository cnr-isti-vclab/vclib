// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PROGRAMS_MACROS_H
#define VCL_BGFX_PROGRAMS_MACROS_H

// clang-format off

#define VCLIB_STRINGIFY(x) #x
#define VCLIB_TOSTRING(x)  VCLIB_STRINGIFY(x)

// VCLIB_BGFX_SHADER creates the include directive for the shader file.
// usage:
// #include VCLIB_BGFX_SHADER(path, profile, file)
// result:
// #include "path/profile/file"
#define VCLIB_BGFX_SHADER(path, profile, file) \
    VCLIB_TOSTRING(path/profile/file)

// VCLIB_JOIN concatenates two tokens with an underscore.
// usage:
// VCLIB_JOIN(token1, token2)
// result:
// token1_token2
#define VCLIB_JOIN2(a, b) a##_##b
#define VCLIB_JOIN(a, b)  VCLIB_JOIN2(a, b)

// clang-format on

#endif // VCL_BGFX_PROGRAMS_MACROS_H
