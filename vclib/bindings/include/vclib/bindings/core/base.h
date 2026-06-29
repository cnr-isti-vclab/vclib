// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_BASE_H
#define VCL_BINDINGS_CORE_BASE_H

#include "base/base.h"
#include "base/exceptions.h"
#include "base/logger.h"
#include "base/timer.h"

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initBase(pybind11::module& m)
{
    initBaseBase(m);
    initBaseExceptions(m);
    initLogger(m);
    initBaseTimer(m);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_BASE_H
