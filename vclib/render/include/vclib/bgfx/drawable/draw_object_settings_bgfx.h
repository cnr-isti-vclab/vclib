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

#ifndef VCL_BGFX_DRAWABLE_DRAW_OBJECT_SETTINGS_BGFX_H
#define VCL_BGFX_DRAWABLE_DRAW_OBJECT_SETTINGS_BGFX_H

#include <vclib/base.h>
#include <vclib/bgfx/environment.h>

namespace vcl {

/**
 * @brief A simple struct containing the settings to draw a drawable object
 * in a bgfx canvas.
 */
struct DrawObjectSettingsBGFX
{
    /**< @brief The object ID to assign to the object. */
    uint objectId = 0;

    /**< @brief The view ID on which to draw the object. */
    uint viewId = 0;

    /**< @brief Option that tells whether the object must be drawn in PBR mode.
     */
    bool pbrMode = false;

    const Environment* environment = nullptr;
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAW_OBJECT_SETTINGS_BGFX_H
