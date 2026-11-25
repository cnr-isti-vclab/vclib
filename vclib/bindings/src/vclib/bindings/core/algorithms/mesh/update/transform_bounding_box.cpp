// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/update/bounding_box.h>
#include <vclib/algorithms/mesh/update/transform.h>

namespace vcl::bind::detail {

void initUpdateTransformBoundingBoxAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            // bounding_box.h

            m.def(
                "update_bounding_box",
                [](MeshType& m) { return updateBoundingBox(m); },
                "mesh"_a);

            // transform.h

            m.def(
                "apply_transform_matrix",
                [](MeshType&             m,
                   const vcl::Matrix44d& mat,
                   bool                  updateNormals = true) {
                    return applyTransformMatrix(m, mat, updateNormals);
                },
                "mesh"_a,
                "matrix"_a,
                "update_normals"_a = true);

            m.def(
                "translate",
                [](MeshType& m, const Point3d& t) { return translate(m, t); },
                "mesh"_a,
                "translate_vector"_a);

            m.def(
                "scale",
                [](MeshType& m, const Point3d& s) { return scale(m, s); },
                "mesh"_a,
                "scale_vector"_a);

            m.def(
                "scale",
                [](MeshType& m, const double& s) { return scale(m, s); },
                "mesh"_a,
                "scale_factor"_a);

            m.def(
                "rotate",
                [](MeshType&             m,
                   const vcl::Matrix33d& mat,
                   bool                  updateNormals = true) {
                    return rotate(m, mat, updateNormals);
                },
                "mesh"_a,
                "matrix"_a,
                "update_normals"_a = true);

            m.def(
                "rotate",
                [](MeshType&           m,
                   const vcl::Point3d& axis,
                   double              angleRad,
                   bool                updateNormals = true) {
                    return rotate(m, axis, angleRad, updateNormals);
                },
                "mesh"_a,
                "axis"_a,
                "angle_rad"_a,
                "update_normals"_a = true);

            m.def(
                "rotate_deg",
                [](MeshType&           m,
                   const vcl::Point3d& axis,
                   double              angleDeg,
                   bool                updateNormals = true) {
                    return rotateDeg(m, axis, angleDeg, updateNormals);
                },
                "mesh"_a,
                "axis"_a,
                "angle_deg"_a,
                "update_normals"_a = true);
        };

    defForAllMeshTypes(m, fAllMeshes);
}

} // namespace vcl::bind::detail
