// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_ALGORITHMS_H
#define VCL_BINDINGS_CORE_ALGORITHMS_H

#include "algorithms/mesh/clean.h"
#include "algorithms/mesh/convex_hull.h"
#include "algorithms/mesh/create.h"
#include "algorithms/mesh/distance.h"
#include "algorithms/mesh/face_topology.h"
#include "algorithms/mesh/import_export.h"
#include "algorithms/mesh/smooth.h"
#include "algorithms/mesh/stat.h"
#include "algorithms/mesh/update.h"

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initAlgorithms(pybind11::module& m)
{
    initCleanAlgorithms(m);
    initConvexHullAlgorithms(m);
    initCreateAlgorithms(m);
    initDistanceAlgorithms(m);
    initFaceTopologyAlgorithms(m);
    initImportExportAlgorithms(m);
    initSmoothAlgorithms(m);
    initStatAlgorithms(m);
    initUpdateAlgorithms(m);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_ALGORITHMS_H
