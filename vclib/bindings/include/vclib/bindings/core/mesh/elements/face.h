// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_ELEMENTS_FACE_H
#define VCL_BINDINGS_CORE_MESH_ELEMENTS_FACE_H

#include <vclib/mesh.h>

#include <vclib/bindings/core/mesh/components.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<MeshConcept MeshType, typename... Options>
void initFace(pybind11::class_<MeshType, Options...>& ct)
{
    using FaceType = MeshType::FaceType;

    // Create the class
    pybind11::class_<FaceType> c(ct, "Face");

    initComponents(c);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_ELEMENTS_FACE_H
