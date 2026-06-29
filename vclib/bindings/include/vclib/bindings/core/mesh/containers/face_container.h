// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_CONTAINERS_FACE_CONTAINER_H
#define VCL_BINDINGS_CORE_MESH_CONTAINERS_FACE_CONTAINER_H

#include "container.h"

#include <vclib/space/core.h>

namespace vcl::bind {

template<FaceMeshConcept MeshType>
void initFaceContainer(pybind11::class_<MeshType>& ct)
{
    namespace py = pybind11;

    using FaceType = MeshType::FaceType;

    initContainer<FaceType>(ct, "face");

    ct.def("add_face", [](MeshType& t, const std::vector<uint>& f) {
        return t.addFace(f);
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_CONTAINERS_FACE_CONTAINER_H
