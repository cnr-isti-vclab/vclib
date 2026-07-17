// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SELECTION_SELECTION_PARAMETERS_BGFX_H
#define VCL_BGFX_SELECTION_SELECTION_PARAMETERS_BGFX_H

#include <vclib/render/selection/selection_mode.h>

#include <vclib/space/core.h>

#include <bgfx/bgfx.h>

namespace vcl {

struct SelectionParameters
{
    uint          drawViewId;  // The view id on which you draw
    uint          pass1ViewId; // visible selection's first pass
    uint          pass2ViewId; // visible selection's compute pass
    Box2d         box;         // selection box
    SelectionMode mode;        // selection mode
    bool isTemporary; // Whether the selection is requested while still deciding
                      // on the final selection box
    bgfx::TextureHandle primIdTex; // texture that will contain the primitive
                                   // ids  when doing visible selection
    bgfx::TextureHandle meshIdTex; // texture that will contain the mesh ids
                                   // when doing visible selection
    Point2<uint> textureSize;      // sizes of the previous 2 textures
    uint         meshId;           // Id of the mesh
};

} // namespace vcl

#endif // VCL_BGFX_SELECTION_SELECTION_PARAMETERS_BGFX_H
