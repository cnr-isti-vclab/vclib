// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H

#include <vclib/base.h>

#include <vector>
#include <functional>

namespace vcl {

class AbstractInputActionMap;

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
     * @brief Retrieves the action maps associated with this editor.
     *
     * This method allows the editor to expose its input action maps (which bind
     * physical inputs like keys or mouse buttons to specific logical actions)
     * to the outside world.
     *
     * The primary use case is for the UI (such as the Settings Dialog) to
     * collect all the action maps from active editors and present them to the
     * user for customization. Modifying the returned maps will dynamically
     * update the editor's input bindings.
     *
     * @return A vector of mutable references to the editor's
     * AbstractInputActionMaps. Returns an empty vector by default if the editor
     * has no custom bindings.
     */
    virtual std::vector<std::reference_wrapper<AbstractInputActionMap>>
    actionMaps()
    {
        return {};
    }

    virtual ~EditorSettings() = default;
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_EDITOR_SETTINGS_H
