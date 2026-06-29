// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms.h>
#include <vclib/bindings/core/base.h>
#include <vclib/bindings/core/io.h>
#include <vclib/bindings/core/meshes.h>
#include <vclib/bindings/core/space.h>

#include <vclib/base.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

// creation of a python module
PYBIND11_MODULE(core, m)
{
    // import the bindings
    using namespace vcl::bind;

    // initialize the bindings
    initBase(m);

    initSpace(m);

    initMeshes(m);

    initAlgorithms(m);

    initIO(m);
}

} // namespace vcl::bind
