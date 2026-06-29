// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/base/exceptions.h>

#include <vclib/base.h>

namespace vcl::bind {

void initBaseExceptions(pybind11::module& m)
{
    namespace py = pybind11;

    py::register_exception<WrongSizeException>(m, "WrongSizeException");
}

} // namespace vcl::bind
