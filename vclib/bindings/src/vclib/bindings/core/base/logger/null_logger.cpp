// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/base/logger/null_logger.h>
#include <vclib/bindings/utils.h>

#include <vclib/base.h>

#include <pybind11/operators.h>

namespace vcl::bind {

void initNullLogger(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<NullLogger, AbstractLogger> c(m, "NullLogger");

    m.attr("null_logger") = &vcl::nullLogger;
}

} // namespace vcl::bind
