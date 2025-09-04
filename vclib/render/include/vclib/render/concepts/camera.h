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

#ifndef VCL_RENDER_CONCEPTS_CAMERA_H
#define VCL_RENDER_CONCEPTS_CAMERA_H

#include <vclib/base.h>
#include <vclib/space/core.h>

namespace vcl {

template<typename T>
concept CameraConcept = requires (
    T&&                                    obj,
    typename RemoveRef<T>::ScalarType      s,
    typename RemoveRef<T>::ScalarType&     sR,
    typename RemoveRef<T>::ProjectionMode  pm,
    typename RemoveRef<T>::ProjectionMode& pmR) {
    // inner types
    typename RemoveRef<T>::ScalarType;
    typename RemoveRef<T>::PointType;
    typename RemoveRef<T>::MatrixType;
    typename RemoveRef<T>::ProjectionMode;

    // constructors
    RemoveRef<T>();

    { obj.center() } -> Point3Concept;
    { obj.eye() } -> Point3Concept;
    { obj.up() } -> Point3Concept;
    { obj.fieldOfView() } -> std::convertible_to<decltype(s)>;
    { obj.projectionMode() } -> std::convertible_to<decltype(pm)>;
    { obj.verticalHeight() } -> std::convertible_to<decltype(s)>;
    { obj.aspectRatio() } -> std::convertible_to<decltype(s)>;
    { obj.nearPlane() } -> std::convertible_to<decltype(s)>;
    { obj.farPlane() } -> std::convertible_to<decltype(s)>;
    { obj.viewMatrix() } -> Matrix44Concept;
    { obj.projectionMatrix() } -> Matrix44Concept;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.reset() } -> std::same_as<void>;
        { obj.fieldOfView() } -> std::same_as<decltype(sR)>;
        { obj.projectionMode() } -> std::same_as<decltype(pmR)>;
        { obj.setFieldOfViewAdaptingEyeDistance(s) } -> std::same_as<void>;
        { obj.verticalHeight() } -> std::same_as<decltype(sR)>;
        { obj.aspectRatio() } -> std::same_as<decltype(sR)>;
        { obj.nearPlane() } -> std::same_as<decltype(sR)>;
        { obj.farPlane() } -> std::same_as<decltype(sR)>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_CAMERA_H
