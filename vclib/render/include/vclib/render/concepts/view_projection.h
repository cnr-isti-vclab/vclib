// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_VIEW_PROJECTION_H
#define VCL_RENDER_CONCEPTS_VIEW_PROJECTION_H

#include "directional_light.h"

namespace vcl {

template<typename T>
concept ViewProjectionConcept = requires (
    T&&                               obj,
    typename RemoveRef<T>::ScalarType s,
    typename RemoveRef<T>::PointType  p) {
    typename RemoveRef<T>::ScalarType;
    typename RemoveRef<T>::PointType;
    typename RemoveRef<T>::MatrixType;

    { obj.viewMatrix() } -> Matrix44Concept;
    { obj.projectionMatrix() } -> Matrix44Concept;

    { obj.light() } -> DirectionalLightConcept;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.reset() } -> std::same_as<void>;

        { obj.focus(p) } -> std::same_as<void>;
        { obj.fitScene(p, s) } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_VIEW_PROJECTION_H
