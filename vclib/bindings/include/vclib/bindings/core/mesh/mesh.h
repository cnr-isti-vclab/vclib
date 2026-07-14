// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_MESH_H
#define VCL_BINDINGS_CORE_MESH_MESH_H

#include "containers.h"
#include "elements.h"

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initMesh(pybind11::module& m, const std::string& name)
{
    namespace py = pybind11;
    using namespace py::literals;

    // Create the class
    pybind11::class_<MeshType, std::shared_ptr<MeshType>> c(m, name.c_str());

    c.def(py::init<>());
    c.def(py::init<MeshType>());

    defCopy(c);

    initVertex(c);
    initVertexContainer(c);

    if constexpr (HasFaces<MeshType>) {
        initFace(c);
        initFaceContainer(c);
    }
    if constexpr (HasEdges<MeshType>) {
        initEdge(c);
        initEdgeContainer(c);
    }

    initComponents(c);

    c.def("is_compact", &MeshType::isCompact);
    c.def("clear", &MeshType::clear);
    c.def("compact", &MeshType::compact);
    c.def(
        "enable_all_optional_components",
        &MeshType::enableAllOptionalComponents);
    c.def(
        "disable_all_optional_components",
        &MeshType::disableAllOptionalComponents);

    auto enableSameOptionalComponentsOfFun =
        []<MeshConcept OtherMeshType>(
            auto& c, OtherMeshType = OtherMeshType()) {
            c.def(
                "enable_same_optional_components_of",
                [](MeshType& m, const OtherMeshType& o) {
                    m.enableSameOptionalComponentsOf(o);
                });
        };
    defForAllMeshTypes(c, enableSameOptionalComponentsOfFun);

    c.def(
        "append",
        [](MeshType& m, const MeshType& o) {
            m.append(o);
        },
        "other_mesh"_a);

    auto importFromFun = []<MeshConcept OtherMeshType>(
                             auto& c,
                             OtherMeshType = OtherMeshType()) {
        c.def("import_from", [](MeshType& m, const OtherMeshType& o) {
            m.importFrom(o);
        });
    };
    defForAllMeshTypes(c, importFromFun);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_MESH_H
