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

#ifndef VCL_EXT_BGFX_CONTEXT_PROGRAM_MANGER_H
#define VCL_EXT_BGFX_CONTEXT_PROGRAM_MANGER_H

#include <array>
#include <map>
#include <string>

#include <vclib/ext/bgfx/load_program.h>

namespace vcl::bgf {

struct VclProgram
{
    enum Enum {
        DRAWABLE_MESH,
        DRAWABLE_AXIS,
        DRAWABLE_DIRECTIONAL_LIGHT,
        DRAWABLE_TRACKBALL,
        FONT_BASIC,
        FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE,
        FONT_DISTANCE_FIELD_DROP_SHADOW,
        FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE,
        FONT_DISTANCE_FIELD_OUTLINE_IMAGE,
        FONT_DISTANCE_FIELD_OUTLINE,
        FONT_DISTANCE_FIELD_SUBPIXEL,
        FONT_DISTANCE_FIELD,

        COUNT
    };
};

class ProgramManager
{
    std::map<std::string, bgfx::ProgramHandle> programs;

    static inline const std::array<std::string, VclProgram::COUNT>
        programNames = {
            "DrawableMesh",
            "DrawableAxis",
            "DrawableDirectionalLight",
            "DrawableTrackball"
            "FontBasic",
            "FontDistanceFieldDropShadowImage",
            "FontDistanceFieldDropShadow",
            "FontDistanceFieldOutlineDropShadowImage",
            "FontDistanceFieldOutlineImage",
            "FontDistanceFieldOutline",
            "FontDistanceFieldSubpixel",
            "FontDistanceField",
    };

public:
    ProgramManager() = default;

    ~ProgramManager();

    bgfx::ProgramHandle getProgram(VclProgram::Enum program);

    bgfx::ProgramHandle getProgram(const std::string& name) const;

    bgfx::ProgramHandle loadProgram(
        const std::string& name,
        const std::string& vs,
        const std::string& fs);

private:
    static bgfx::ProgramHandle loadProgram(VclProgram::Enum program);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_CONTEXT_PROGRAM_MANGER_H
