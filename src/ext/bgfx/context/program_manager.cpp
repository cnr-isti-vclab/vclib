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

#include <stdexcept>

#include <vclib/ext/bgfx/context/program_manger.h>

namespace vcl::bgf {

ProgramManager::~ProgramManager()
{
    for (const auto& [name, program] : programs) {
        if (bgfx::isValid(program)) {
            bgfx::destroy(program);
        }
    }
}

bgfx::ProgramHandle ProgramManager::getProgram(VclProgram::Enum program)
{
    auto it = programs.find(programNames[program]);
    if (it != programs.end()) [[likely]] {
        return it->second;
    }
    else [[unlikely]] {
        bgfx::ProgramHandle p           = loadProgram(program);
        programs[programNames[program]] = p;
        return p;
    }
}

bgfx::ProgramHandle ProgramManager::getProgram(const std::string& name) const
{
    return programs.at(name);
}

bgfx::ProgramHandle ProgramManager::loadProgram(
    const std::string& name,
    const std::string& vs,
    const std::string& fs)
{
    if (programs.find(name) != programs.end()) [[unlikely]] {
        throw std::runtime_error("A program named " + name + " already exists");
    }
    bgfx::ProgramHandle p = bgf::loadProgram(vs, fs);
    programs[name]        = p;
    return p;
}

bgfx::ProgramHandle ProgramManager::loadProgram(VclProgram::Enum program)
{
    switch (program) {
    case VclProgram::DRAWABLE_MESH:
        return bgf::loadProgram(
            "vclib/ext/bgfx/drawable_mesh/vs_drawable_mesh",
            "vclib/ext/bgfx/drawable_mesh/fs_drawable_mesh");

    case VclProgram::DRAWABLE_AXIS:
        return bgf::loadProgram(
            "vclib/ext/bgfx/drawable_axis/vs_drawable_axis",
            "vclib/ext/bgfx/drawable_axis/fs_drawable_axis");

    case VclProgram::DRAWABLE_DIRECTIONAL_LIGHT:
        return bgf::loadProgram(
            "vclib/ext/bgfx/drawable_directional_light/"
            "vs_drawable_directional_light",
            "vclib/ext/bgfx/drawable_directional_light/"
            "fs_drawable_directional_light");

    case VclProgram::FONT_BASIC:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_basic",
            "vclib/ext/bgfx/font/fs_font_basic");

    case VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_distance_field_drop_shadow_image",
            "vclib/ext/bgfx/font/fs_font_distance_field_drop_shadow_image");

    case VclProgram::FONT_DISTANCE_FIELD_DROP_SHADOW:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_distance_field_drop_shadow",
            "vclib/ext/bgfx/font/fs_font_distance_field_drop_shadow");

    case VclProgram::FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/"
            "vs_font_distance_field_outline_drop_shadow_image",
            "vclib/ext/bgfx/font/"
            "fs_font_distance_field_outline_drop_shadow_image");

    case VclProgram::FONT_DISTANCE_FIELD_OUTLINE_IMAGE:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_distance_field_outline_image",
            "vclib/ext/bgfx/font/fs_font_distance_field_outline_image");

    case VclProgram::FONT_DISTANCE_FIELD_OUTLINE:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_distance_field_outline",
            "vclib/ext/bgfx/font/fs_font_distance_field_outline");

    case VclProgram::FONT_DISTANCE_FIELD_SUBPIXEL:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_distance_field_subpixel",
            "vclib/ext/bgfx/font/fs_font_distance_field_subpixel");

    case VclProgram::FONT_DISTANCE_FIELD:
        return bgf::loadProgram(
            "vclib/ext/bgfx/font/vs_font_distance_field",
            "vclib/ext/bgfx/font/fs_font_distance_field");

    case VclProgram::COUNT: return BGFX_INVALID_HANDLE;
    }

    return BGFX_INVALID_HANDLE;
}

} // namespace vcl::bgf
