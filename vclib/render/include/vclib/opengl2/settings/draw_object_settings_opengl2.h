// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_SETTINGS_DRAW_OBJECT_SETTINGS_OPENGL2_H
#define VCL_OPENGL2_SETTINGS_DRAW_OBJECT_SETTINGS_OPENGL2_H

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

#endif // VCL_OPENGL2_SETTINGS_DRAW_OBJECT_SETTINGS_OPENGL2_H
