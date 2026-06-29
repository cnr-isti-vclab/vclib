// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
