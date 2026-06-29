// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_TEXT_EMBEDDED_FONTS_DROID_SANS_H
#define VCL_BGFX_TEXT_EMBEDDED_FONTS_DROID_SANS_H

#include "embedded_font.h"

namespace vcl {

template<>
struct EmbeddedFont<VclFont::DROID_SANS>
{
    static FontData embeddedFont();
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_EMBEDDED_FONTS_DROID_SANS_H
