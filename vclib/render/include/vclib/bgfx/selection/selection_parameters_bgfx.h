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
/**
 * The parameters are as follows:
 *   - The view id on which you draw
 *   - The view id of the visible selection's first pass
 *   - The view id of the visible selection's compute pass
 *   - The selection box
 *   - The selection mode
 *   - Whether the selection is requested while still deciding on the final selection box
 *   - The texture handle for the texture that will contain the primitive ids when doing visible selection
 *   - The texture handle for the texture that will contain the mesh ids when doing visible selection
 *   - The XY sizes of the previous 2 textures (yes, it should be the same for both)
 *   - The Id of the mesh (useful only for visible selection; should start from 1)
 */
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
