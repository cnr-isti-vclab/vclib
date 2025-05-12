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

#include <vclib/bindings/core/algorithms.h>
#include <vclib/bindings/core/io.h>
#include <vclib/bindings/core/meshes.h>
#include <vclib/bindings/core/space.h>

#include <vclib/types.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

// creation of a python module
PYBIND11_MODULE(core, m)
{
    // import the bindings
    using namespace vcl::bind;

    m.attr("UINT_NULL") = pybind11::int_(vcl::UINT_NULL);

    // initialize the bindings
    initSpace(m);

    initMeshes(m);

    initAlgorithms(m);

    initIO(m);
}

} // namespace vcl::bind
