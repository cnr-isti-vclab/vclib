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

#ifndef VCL_SELECTION_PARAMETERS_BGFX_H
#define VCL_SELECTION_PARAMETERS_BGFX_H

#include <bgfx/bgfx.h>
#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selection_mode.h>

#include <array>

namespace vcl {
struct SelectionParameters
{
    uint                drawViewId;
    uint                pass1ViewId;
    uint                pass2ViewId;
    SelectionBox        box;
    SelectionMode       mode;
    bool                isTemporary;
    bgfx::TextureHandle primIdTex;
    bgfx::TextureHandle meshIdTex;
    std::array<uint, 2> texAttachmentsSize;
    uint                meshId;
};
} // namespace vcl

#endif
