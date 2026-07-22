// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_DRAWABLE_OBJECT_H
#define VCL_RENDER_CONCEPTS_DRAWABLE_OBJECT_H

#include <vclib/render/settings/draw_object_settings.h>
#include <vclib/space/core.h>

namespace vcl {

template<typename T>
concept DrawableObjectConcept =
    requires (T&& obj, uint u, DrawObjectSettings s) {
        { obj.boundingBox() } -> Box3Concept;
        { obj.isVisible() } -> std::same_as<bool>;
        { obj.name() } -> std::convertible_to<std::string>;
        { obj.info() } -> std::convertible_to<std::string>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.init() } -> std::same_as<void>;
            { obj.setVisibility(bool()) } -> std::same_as<void>;
            { obj.draw(s) } -> std::same_as<void>;
            { obj.name() } -> std::same_as<std::string&>;
            { obj.info() } -> std::same_as<std::string&>;
        };
    };

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_DRAWABLE_OBJECT_H
