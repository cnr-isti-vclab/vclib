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

#ifndef VCL_OPENGL2_DRAWABLE_DRAW_OBJECT_SETTINGS_OPENGL2_H
#define VCL_OPENGL2_DRAWABLE_DRAW_OBJECT_SETTINGS_OPENGL2_H

#include <vclib/base.h>

namespace vcl {

/**
 * @brief A simple struct containing the settings to draw a drawable object
 * in a opengl2 canvas.
 */
struct DrawObjectSettingsOpenGL2
{
    /**< @brief The object ID to assign to the object. */
    uint objectId = 0;
};

} // namespace vcl

#endif // VCL_OPENGL2_DRAWABLE_DRAW_OBJECT_SETTINGS_OPENGL2_H
