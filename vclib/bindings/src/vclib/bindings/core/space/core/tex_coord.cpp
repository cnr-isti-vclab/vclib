// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/tex_coord.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initTexCoord(pybind11::module& m)
{
    namespace py = pybind11;

    using Scalar = double;
    using P      = TexCoord<Scalar>;

    py::class_<P> c(m, "TexCoord");

    populateTexCoord<P>(c);
}

} // namespace vcl::bind
