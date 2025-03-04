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

#ifndef VCL_BINDINGS_MESH_MESH_H
#define VCL_BINDINGS_MESH_MESH_H

#include "containers.h"
#include "elements.h"

#include <vclib/bindings/utils.h>

#include <vclib/concepts/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initMesh(pybind11::module& m, const std::string& name)
{
    namespace py = pybind11;

    // Create the class
    pybind11::class_<MeshType> c(m, name.c_str());

    c.def(py::init<>());

    defCopy(c);

    initVertex(c);
    initVertexContainer(c);

    if constexpr (HasFaces<MeshType>) {
        initFace(c);
        initFaceContainer(c);
    }

    initComponents(c);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_MESH_H
