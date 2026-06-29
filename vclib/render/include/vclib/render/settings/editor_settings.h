// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H

#include <vclib/base.h>

namespace vcl {

struct EditorSettings
{
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
