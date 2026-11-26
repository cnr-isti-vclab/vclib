// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/intersection.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

namespace vcl::bind {

namespace detail {

template<FaceMeshConcept InMeshType, EdgeMeshConcept OutMeshType>
void defPlaneIntersection(pybind11::module& m, const std::string& suffix = "")
{
    using namespace pybind11::literals;

    std::string name = "intersection" + suffix;

    m.def(
        name.c_str(),
        [](const InMeshType& m, const Planed& plane) {
            return vcl::intersection<OutMeshType>(m, plane);
        },
        "mesh"_a,
        "plane"_a);
}

template<FaceMeshConcept InMeshType>
void defAllPlaneIntersections(pybind11::module& m)
{
    // default OutMeshType = EdgeMesh
    defPlaneIntersection<InMeshType, vcl::EdgeMesh>(m);

    // typed variants for all EdgeMeshConcept output types
    auto fOutEdgeMeshes = []<EdgeMeshConcept OutMeshType>(
                              pybind11::module& m,
                              OutMeshType = OutMeshType()) {
        std::string suffix =
            "_" + camelCaseToSnakeCase(meshTypeName<OutMeshType>());
        defPlaneIntersection<InMeshType, OutMeshType>(m, suffix);
    };

    defForAllMeshTypes(m, fOutEdgeMeshes);
}

} // namespace detail

void initIntersectionAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defAllPlaneIntersections<MeshType>(m);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fTriMeshes = []<TriangleMeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "intersection",
            [](const MeshType& m, const Sphered& sphere) {
                return vcl::intersection(m, sphere);
            },
            "mesh"_a,
            "sphere"_a);

        m.def(
            "intersection",
            [](const MeshType& m, const Sphered& sphere, double tol) {
                return vcl::intersection(m, sphere, tol);
            },
            "mesh"_a,
            "sphere"_a,
            "tol"_a);
    };

    defForAllMeshTypes(m, fTriMeshes);
}

} // namespace vcl::bind
