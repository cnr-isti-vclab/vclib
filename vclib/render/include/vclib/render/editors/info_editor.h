// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_INFO_EDITOR_H
#define VCL_RENDER_EDITORS_INFO_EDITOR_H

#include "editor.h"

#include <iostream>

namespace vcl {

template<typename ViewerDrawer>
class InfoEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

public:
    // Editor implementation

    void draw(uint /*viewId*/) override {}

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        bool block = Base::onMousePress(button, x, y, modifiers);

        if (!block && button == vcl::MouseButton::LEFT) {
            block = true; // consume the event to prevent further propagation
            auto callback = [](ushort objectId, ushort elementType, uint elementId) {
                if (objectId == 0xFFFF)
                    return;

                std::cerr << "InfoEditor:\n"
                          << "  Object ID:    " << objectId << "\n"
                          << "  Element Type: " << elementType << "\n"
                          << "  Element ID:   " << elementId << "\n";
            };

            Base::viewerReadElementIdRequest(x, y, callback);
        }
        return block;
    }
};

} // namespace vcl

#endif // VCL_RENDER_EDITORS_INFO_EDITOR_H
