// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PROGRAMS_LOAD_PROGRAM_H
#define VCL_BGFX_PROGRAMS_LOAD_PROGRAM_H

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

#include <string>

namespace vcl {

bgfx::ShaderHandle loadShader(const std::string& name);

bgfx::ShaderHandle loadShader(const bgfx::EmbeddedShader::Data& data);

bgfx::ProgramHandle loadProgram(
    const std::string& vsName,
    const std::string& fsName);

bgfx::ProgramHandle loadProgram(
    const bgfx::EmbeddedShader& vs,
    const bgfx::EmbeddedShader& fs,
    bgfx::RendererType::Enum    type);

bgfx::ProgramHandle createProgram(
    bgfx::ShaderHandle vsHandle,
    bgfx::ShaderHandle fsHandle);

bgfx::ProgramHandle createProgram(bgfx::ShaderHandle csHandle);

} // namespace vcl

#endif // VCL_BGFX_PROGRAMS_LOAD_PROGRAM_H
