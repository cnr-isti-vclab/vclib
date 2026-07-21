// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_SPACE_CORE_H
#define VCL_BINDINGS_CORE_SPACE_CORE_H

#include "core/box.h"
#include "core/camera.h"
#include "core/color.h"
#include "core/histogram.h"
#include "core/image.h"
#include "core/material.h"
#include "core/matrix.h"
#include "core/point.h"
#include "core/principal_curvature.h"
#include "core/sphere.h"
#include "core/tex_coord.h"
#include "core/tex_coord_indexed.h"
#include "core/texture_descriptor.h"

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initCore(pybind11::module& m)
{
    namespace py = pybind11;

    // py::module_ sm = m.def_submodule("core", "Core Spatial Data Structures");
    initPoint(m);

    initBox(m);
    initCamera(m);
    initColor(m);
    initHistogram(m);
    initImage(m);
    initMaterial(m);
    initMatrix(m);
    initPrincipalCurvature(m);
    initSphere(m);
    initTexCoord(m);
    initTexCoordIndexed(m);
    initTextureDescriptor(m);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_SPACE_CORE_H
