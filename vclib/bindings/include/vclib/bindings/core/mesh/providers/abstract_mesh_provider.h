// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_PROVIDERS_ABSTRACT_MESH_PROVIDER_H
#define VCL_BINDINGS_CORE_MESH_PROVIDERS_ABSTRACT_MESH_PROVIDER_H

#include <vclib/mesh/providers/abstract_mesh_provider.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initAbstractMeshProvider(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<vcl::AbstractMeshProvider> provider(
        m,
        "AbstractMeshProvider",
        "An abstract interface exposing mesh data in a type-erased way.");

    provider.def("vertex_count", &vcl::AbstractMeshProvider::vertexCount);
    provider.def("face_count", &vcl::AbstractMeshProvider::faceCount);
    provider.def("edge_count", &vcl::AbstractMeshProvider::edgeCount);

    provider.def("face_vertices", &vcl::AbstractMeshProvider::faceVertices);
    provider.def("edge_vertices", &vcl::AbstractMeshProvider::edgeVertices);

    provider.def("face_positions", &vcl::AbstractMeshProvider::facePositions);
    provider.def("edge_positions", &vcl::AbstractMeshProvider::edgePositions);
    provider.def("vertex_position", &vcl::AbstractMeshProvider::vertexPosition);

    provider.def(
        "selected_vertex_count",
        &vcl::AbstractMeshProvider::selectedVertexCount);
    provider.def(
        "selected_face_count", &vcl::AbstractMeshProvider::selectedFaceCount);
    provider.def(
        "selected_edge_count", &vcl::AbstractMeshProvider::selectedEdgeCount);

    provider.def(
        "transform_matrix", &vcl::AbstractMeshProvider::transformMatrix);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_PROVIDERS_ABSTRACT_MESH_PROVIDER_H
