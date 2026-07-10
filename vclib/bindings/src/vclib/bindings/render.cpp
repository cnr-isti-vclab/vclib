// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/drawable/mesh.h>

#include <vclib/base.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

// creation of a python module
PYBIND11_MODULE(render, m)
{
    // import the bindings
    using namespace vcl::bind;

    initDrawableMeshes(m);
}

} // namespace vcl::bind
