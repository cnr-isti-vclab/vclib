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

#ifndef VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H

#include <vclib/base.h>

namespace vcl {

struct EditorSettings {

    enum class EditMode {
        NONE,
        CURRENT_OBJECT,
        VISIBLE_OBJECTS,
        ALL_OBJECTS,
    };

    /**< @brief The edit mode of the editor. */
    EditMode editMode = EditMode::CURRENT_OBJECT;

    /**
     * @brief Additional custom settings. Each editor can define its own custom
     * settings, that can be used to store additional information about the
     * editor (e.g. the color of a bounding box, or the size of a gizmo). The
     * editor is responsible for defining the keys and the types of the values
     * stored in this map, and for using them in the editor itself (e.g. to draw
     * the bounding box with).
     */
    std::map<std::string, std::any> customSettings;
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H
