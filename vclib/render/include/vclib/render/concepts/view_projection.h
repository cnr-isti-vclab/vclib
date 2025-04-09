/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_RENDER_CONCEPTS_VIEW_PROJECTION_H
#define VCL_RENDER_CONCEPTS_VIEW_PROJECTION_H

#include "camera.h"
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

    { obj.camera() } -> CameraConcept;
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
