/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/bgfx/context/program_manager.h>

#include <vclib/bgfx/context/embedded_shaders.h>
#include <vclib/bgfx/context/load_program.h>

#include <stdexcept>

namespace vcl {

ProgramManager::~ProgramManager()
{
    for (const auto& [name, program] : mPrograms) {
        if (bgfx::isValid(program)) {
            bgfx::destroy(program);
        }
    }
}

bgfx::ProgramHandle ProgramManager::getProgram(VclProgram::Enum program)
{
    auto it = mPrograms.find(VclProgram::programNames[program]);
    if (it != mPrograms.end()) [[likely]] {
        return it->second;
    }
    else [[unlikely]] {
        bgfx::ProgramHandle p = loadProgram(program, mRenderType);
        mPrograms[VclProgram::programNames[program]] = p;
        return p;
    }
}

bgfx::ProgramHandle ProgramManager::getProgram(const std::string& name) const
{
    return mPrograms.at(name);
}

bgfx::ProgramHandle ProgramManager::loadProgram(
    const std::string& name,
    const std::string& vs,
    const std::string& fs)
{
    if (mPrograms.find(name) != mPrograms.end()) [[unlikely]] {
        throw std::runtime_error("A program named " + name + " already exists");
    }
    bgfx::ProgramHandle p = vcl::loadProgram(vs, fs);
    mPrograms[name]       = p;
    return p;
}

bgfx::ProgramHandle ProgramManager::loadProgram(
    VclProgram::Enum         program,
    bgfx::RendererType::Enum type)
{
    switch (program) {
    case VclProgram::DRAWABLE_MESH:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::DRAWABLE_MESH>::vertexEmbeddedShader(
                    type)),
            vcl::loadShader(
                EmbeddedShader<
                    VclProgram::DRAWABLE_MESH>::fragmentEmbeddedShader(type)));

    case VclProgram::DRAWABLE_AXIS:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::DRAWABLE_AXIS>::vertexEmbeddedShader(
                    type)),
            vcl::loadShader(
                EmbeddedShader<
                    VclProgram::DRAWABLE_AXIS>::fragmentEmbeddedShader(type)));

    case VclProgram::DRAWABLE_DIRECTIONAL_LIGHT:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::DRAWABLE_DIRECTIONAL_LIGHT>::
                    vertexEmbeddedShader(type)),
            vcl::loadShader(
                EmbeddedShader<VclProgram::DRAWABLE_DIRECTIONAL_LIGHT>::
                    fragmentEmbeddedShader(type)));

    case VclProgram::DRAWABLE_TRACKBALL:
        return vcl::createProgram(
            vcl::loadShader(EmbeddedShader<VclProgram::DRAWABLE_TRACKBALL>::
                                vertexEmbeddedShader(type)),
            vcl::loadShader(EmbeddedShader<VclProgram::DRAWABLE_TRACKBALL>::
                                fragmentEmbeddedShader(type)));

    case VclProgram::FONT_BASIC:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_BASIC>::vertexEmbeddedShader(
                    type)),
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_BASIC>::fragmentEmbeddedShader(
                    type)));

    case VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE:
        return vcl::createProgram(
            vcl::loadShader(EmbeddedShader<
                            VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE>::
                                vertexEmbeddedShader(type)),
            vcl::loadShader(EmbeddedShader<
                            VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE>::
                                fragmentEmbeddedShader(type)));

    case VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW>::
                    vertexEmbeddedShader(type)),
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW>::
                    fragmentEmbeddedShader(type)));

    case VclProgram::FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<
                    VclProgram::FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE>::
                    vertexEmbeddedShader(type)),
            vcl::loadShader(
                EmbeddedShader<
                    VclProgram::FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE>::
                    fragmentEmbeddedShader(type)));

    case VclProgram::FONT_DISTANCE_FIELD_OUTLINE_IMAGE:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_OUTLINE_IMAGE>::
                    vertexEmbeddedShader(type)),
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_OUTLINE_IMAGE>::
                    fragmentEmbeddedShader(type)));

    case VclProgram::FONT_DISTANCE_FIELD_OUTLINE:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_OUTLINE>::
                    vertexEmbeddedShader(type)),
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_OUTLINE>::
                    fragmentEmbeddedShader(type)));

    case VclProgram::FONT_DISTANCE_FIELD_SUBPIXEL:
        return vcl::createProgram(
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_SUBPIXEL>::
                    vertexEmbeddedShader(type)),
            vcl::loadShader(
                EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD_SUBPIXEL>::
                    fragmentEmbeddedShader(type)));

    case VclProgram::FONT_DISTANCE_FIELD:
        return vcl::createProgram(
            vcl::loadShader(EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD>::
                                vertexEmbeddedShader(type)),
            vcl::loadShader(EmbeddedShader<VclProgram::FONT_DISTANCE_FIELD>::
                                fragmentEmbeddedShader(type)));

    case VclProgram::COUNT: return BGFX_INVALID_HANDLE;
    }

    return BGFX_INVALID_HANDLE;
}

} // namespace vcl
