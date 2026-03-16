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

#ifndef VCL_BGFX_EDITORS_AXIS_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_AXIS_EDITOR_BGFX_H

#include <vclib/bgfx/drawable/drawable_axis.h>
#include <vclib/render/editors/editor.h>

namespace vcl {

template<typename ViewerDrawer>
class AxisEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    DrawableAxis mAxis;

public:
    AxisEditorBGFX() {};

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    void draw(uint viewId) const override
    {
        DrawObjectSettings settings;
        settings.viewId = viewId;

        mAxis.draw(settings);
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_AXIS_EDITOR_BGFX_H
