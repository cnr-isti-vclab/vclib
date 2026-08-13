// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H

#include <vclib/render/input/binding_map.h>
#include <vclib/render/input/input.h>
#include <vclib/render/selection/selection_mode.h>
#include <vclib/render/settings/editor_settings.h>

#include <vclib/space/core.h>

namespace vcl {

struct SelectionEditorSettings : public EditorSettings
{
    using KeyMap =
        BindingMap<std::pair<Key::Enum, KeyModifiers>, SelectionAtomicAction>;
    using MouseMap = BindingMap<
        std::pair<MouseButton::Enum, KeyModifiers>,
        SelectionDragAction>;

    bool       selectVertices    = false;
    bool       selectFaces       = false;
    bool       onlyVisible       = false;
    vcl::Color selectionBoxColor = vcl::Color(27, 120, 249, 64);

    KeyMap   keyBindings   = defaultKeyMap();
    MouseMap mouseBindings = defaultMouseMap();

private:
    static KeyMap defaultKeyMap()
    {
        using enum Key::Enum;
        using enum KeyModifier::Enum;

        return KeyMap {
            {{A, {CONTROL}}, SelectionAtomicAction::ALL   },
            {{D, {CONTROL}}, SelectionAtomicAction::NONE  },
            {{I, {CONTROL}}, SelectionAtomicAction::INVERT}
        };
    }

    static MouseMap defaultMouseMap()
    {
        using enum MouseButton::Enum;
        using enum KeyModifier::Enum;

        return MouseMap {
            {{LEFT, {NO_MODIFIER}},    SelectionDragAction::REGULAR },
            {{LEFT, {CONTROL}},        SelectionDragAction::ADD     },
            {{LEFT, {CONTROL, SHIFT}}, SelectionDragAction::SUBTRACT}
        };
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H
