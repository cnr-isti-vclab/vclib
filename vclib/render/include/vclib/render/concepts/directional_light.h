// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_DIRECTIONAL_LIGHT_H
#define VCL_RENDER_CONCEPTS_DIRECTIONAL_LIGHT_H

#include <vclib/space/core.h>

namespace vcl {

template<typename T>
concept DirectionalLightConcept =
    requires (T&& obj, RemoveRef<T>::PointType p, vcl::Color c) {
        // types
        typename RemoveRef<T>::PointType;

        // constructors
        RemoveRef<T>();
        RemoveRef<T>(p, c);

        { obj.direction() } -> Point3Concept;
        { obj.color() } -> ColorConcept;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.reset() } -> std::same_as<void>;
        };
    };

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_DIRECTIONAL_LIGHT_H
