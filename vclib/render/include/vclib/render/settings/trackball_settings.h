// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_TRACKBALL_SETTINGS_H
#define VCL_RENDER_SETTINGS_TRACKBALL_SETTINGS_H

#include <vclib/render/input.h>
#include <vclib/render/viewer/trackball.h>

namespace vcl {

/**
 * @brief Contains the settings and bindings for the trackball.
 */
struct TrackballSettings
{
    using DragMotionMap = InputActionMap<
        std::pair<MouseButton::Enum, KeyModifiers>,
        TrackballMotionType>;
    using ScrollAtomicMap = InputActionMap<
        std::pair<ScrollAxis::Enum, KeyModifiers>,
        TrackballMotionType>;
    using KeyAtomicMap =
        InputActionMap<std::pair<Key::Enum, KeyModifiers>, std::string>;

    DragMotionMap   dragMotionMap   = defaultDragMotionMap();
    ScrollAtomicMap scrollAtomicMap = defaultScrollMotionMap();
    KeyAtomicMap    keyAtomicMap    = defaultKeyAtomicMap();

private:
    static DragMotionMap defaultDragMotionMap()
    {
        using enum MouseButton::Enum;
        using enum KeyModifier::Enum;
        using enum TrackballMotionType;
        using Input = std::pair<MouseButton::Enum, KeyModifiers>;

        DragMotionMap map("Trackball Drag Motions");
        map.registerActions({
            {ARC,           "Arcball Rotation",   Input {LEFT, {NO_MODIFIER}}   },
            {PAN,           "Pan",                Input {LEFT, {CONTROL}}       },
            {ZMOVE,         "Zoom (Translation)", Input {LEFT, {ALT}}           },
            {SCALE,         "Scale",              Input {LEFT, {SHIFT}}         },
            {ROLL,          "Roll",               Input {MIDDLE, {CONTROL}}     },
            {DIR_LIGHT_ARC, "Light Rotation",     Input {LEFT, {SHIFT, CONTROL}}}
        });
        return map;
    }

    static ScrollAtomicMap defaultScrollMotionMap()
    {
        using enum KeyModifier::Enum;
        using enum TrackballMotionType;
        using Input = std::pair<ScrollAxis::Enum, KeyModifiers>;

        ScrollAtomicMap map("Trackball Scroll Motions");
        map.registerActions({
            {SCALE, "Scale",         Input {ScrollAxis::VERTICAL, {NO_MODIFIER}}},
            {ROLL,  "Roll",          Input {ScrollAxis::VERTICAL, {CONTROL}}    },
            {FOV,   "Field of View", Input {ScrollAxis::VERTICAL, {SHIFT}}      }
        });
        return map;
    }

    static KeyAtomicMap defaultKeyAtomicMap()
    {
        using enum Key::Enum;
        using enum KeyModifier::Enum;
        using Input = std::pair<Key::Enum, KeyModifiers>;

        KeyAtomicMap map("Trackball Key Motions");
        map.registerActions({
            {"Reset Trackball",         "Reset Trackball", Input {R, {NO_MODIFIER}}      },
            {"Reset Directional Light",
             "Reset Directional Light",                    Input {R, {CONTROL, SHIFT}}   },
            {"Rotate X+",               "Rotate X+",       Input {NP_2, {NO_MODIFIER}}   },
            {"Rotate Y-",               "Rotate Y-",       Input {NP_4, {NO_MODIFIER}}   },
            {"Rotate Y+",               "Rotate Y+",       Input {NP_6, {NO_MODIFIER}}   },
            {"Rotate X-",               "Rotate X-",       Input {NP_8, {NO_MODIFIER}}   },
            {"Translate Y+",            "Translate Y+",    Input {UP, {NO_MODIFIER}}     },
            {"Translate Y-",            "Translate Y-",    Input {DOWN, {NO_MODIFIER}}   },
            {"Translate X-",            "Translate X-",    Input {LEFT, {NO_MODIFIER}}   },
            {"Translate X+",            "Translate X+",    Input {RIGHT, {NO_MODIFIER}}  },
            {"View Front",              "View Front",      Input {NP_1, {NO_MODIFIER}}   },
            {"View Top",                "View Top",        Input {NP_7, {NO_MODIFIER}}   },
            {"View Right",              "View Right",      Input {NP_3, {NO_MODIFIER}}   },
            {"View Back",               "View Back",       Input {NP_1, {CONTROL}}       },
            {"View Bottom",             "View Bottom",     Input {NP_7, {CONTROL}}       },
            {"View Left",               "View Left",       Input {NP_3, {CONTROL}}       },
            {"Toggle Projection",
             "Toggle Projection",                          Input {NP_5, {NO_MODIFIER}}   },
            {"Rotate Light X+",
             "Rotate Light X+",                            Input {NP_2, {CONTROL, SHIFT}}},
            {"Rotate Light Y-",
             "Rotate Light Y-",                            Input {NP_4, {CONTROL, SHIFT}}},
            {"Rotate Light Y+",
             "Rotate Light Y+",                            Input {NP_6, {CONTROL, SHIFT}}},
            {"Rotate Light X-",
             "Rotate Light X-",                            Input {NP_8, {CONTROL, SHIFT}}}
        });
        return map;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_TRACKBALL_SETTINGS_H
