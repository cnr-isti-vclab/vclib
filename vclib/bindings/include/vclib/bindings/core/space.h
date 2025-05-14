/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_BINDINGS_CORE_SPACE_H
#define VCL_BINDINGS_CORE_SPACE_H

#include "space/complex.h"
#include "space/core.h"

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initSpace(pybind11::module& m)
{
    namespace py = pybind11;

    // py::module_ sm = m.def_submodule("space", "Spatial Data Structures");
    initCore(m);
    initComplex(m);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_SPACE_H
