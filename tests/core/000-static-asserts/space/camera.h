/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef CAMERA_H
#define CAMERA_H

#include <vclib/space/core.h>

void cameraStaticAsserts()
{
    using namespace vcl;

    using Camerad = Camera<double>;

    static_assert(
        GenericCameraConcept<Cameraf>,
        "Camera does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<const Cameraf>,
        "const Camera does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<Cameraf&>,
        "Camera& does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<const Cameraf&>,
        "const Camera& does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<Cameraf&&>,
        "Camera&& does not satisfy the GenericCameraConcept");

    static_assert(
        GenericCameraConcept<Camerad>,
        "Camera<double> does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<const Camerad>,
        "const Camera<double> does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<Camerad&>,
        "Camera<double>& does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<const Camerad&>,
        "const Camera<double>& does not satisfy the GenericCameraConcept");
    static_assert(
        GenericCameraConcept<Camerad&&>,
        "Camera<double>&& does not satisfy the GenericCameraConcept");

    static_assert(
        CameraConcept<Cameraf>, "Camera does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<const Cameraf>,
        "const Camera does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<Cameraf&>, "Camera& does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<const Cameraf&>,
        "const Camera& does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<Cameraf&&>,
        "Camera&& does not satisfy the CameraConcept");

    static_assert(
        CameraConcept<Camerad>,
        "Camera<double> does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<const Camerad>,
        "const Camera<double> does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<Camerad&>,
        "Camera<double>& does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<const Camerad&>,
        "const Camera<double>& does not satisfy the CameraConcept");
    static_assert(
        CameraConcept<Camerad&&>,
        "Camera<double>&& does not satisfy the CameraConcept");

    static_assert(Serializable<Cameraf>, "Camera is not serializable");
    static_assert(Serializable<Camerad>, "Camera<double> is not serializable");
}

#endif // CAMERA_H
