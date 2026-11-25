// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/update/curvature.h>
#include <vclib/algorithms/mesh/update/quality.h>

namespace vcl::bind::detail {

void initUpdateCurvatureQualityAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def(
                "set_vertex_quality",
                [](MeshType& m, double quality) {
                    return setVertexQuality(m, quality);
                },
                "mesh"_a,
                "quality"_a = 0.0);

            m.def(
                "clamp_vertex_quality",
                [](MeshType& m, double minQ, double maxQ) {
                    return clampVertexQuality(m, minQ, maxQ);
                },
                "mesh"_a,
                "min_quality"_a = 0.0,
                "max_quality"_a = 1.0);

            m.def(
                "normalize_vertex_quality",
                [](MeshType& m, double minQ, double maxQ) {
                    return normalizeVertexQuality(m, minQ, maxQ);
                },
                "mesh"_a,
                "min_quality"_a = 0.0,
                "max_quality"_a = 1.0);
        };

    defForAllMeshTypes(m, fAllMeshes);

    // enum for PrincipalCurvatureAlgorithm (define it here because is not
    // related to the mesh definition)
    py::enum_<vcl::PrincipalCurvatureAlgorithm> pcaEnum(
        m, "PrincipalCurvatureAlgorithm");
    pcaEnum.value("TAUBIN95", vcl::PrincipalCurvatureAlgorithm::TAUBIN95);
    pcaEnum.value("PCA", vcl::PrincipalCurvatureAlgorithm::PCA);
    pcaEnum.export_values();

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // curvature.h

        m.def(
            "update_vertex_principal_curvature",
            [](MeshType&                   m,
               PrincipalCurvatureAlgorithm alg,
               double                      radius,
               bool                        montecarloSampling,
               AbstractLogger&             log) {
                using enum PrincipalCurvatureAlgorithm;
                switch (alg) {
                case TAUBIN95: updatePrincipalCurvatureTaubin95(m, log); break;
                case PCA:
                    if (radius <= 0)
                        radius = boundingBox(m).diagonal() * 0.1;
                    updatePrincipalCurvaturePCA(
                        m, radius, montecarloSampling, log);
                }
            },
            "mesh"_a,
            "algorithm"_a           = PrincipalCurvatureAlgorithm::TAUBIN95,
            "radius"_a              = -1.0,
            "montecarlo_sampling"_a = true,
            "log"_a                 = py::cast(&vcl::nullLogger));

        // quality.h

        m.def(
            "set_face_quality",
            [](MeshType& m, double quality) {
                return vcl::setFaceQuality(m, quality);
            },
            "mesh"_a,
            "quality"_a = 0.0);

        m.def(
            "clamp_face_quality",
            [](MeshType& m, double minQ, double maxQ) {
                return vcl::clampFaceQuality(m, minQ, maxQ);
            },
            "mesh"_a,
            "min_quality"_a = 0.0,
            "max_quality"_a = 1.0);

        m.def(
            "normalize_face_quality",
            [](MeshType& m, double minQ, double maxQ) {
                return vcl::normalizeFaceQuality(m, minQ, maxQ);
            },
            "mesh"_a,
            "min_quality"_a = 0.0,
            "max_quality"_a = 1.0);

        m.def(
            "set_vertex_quality_from_vertex_valence",
            [](MeshType& m) {
                return vcl::setVertexQualityFromVertexValence(m);
            },
            "mesh"_a);

        m.def(
            "set_face_quality_from_face_area",
            [](MeshType& m) { return vcl::setFaceQualityFromFaceArea(m); },
            "mesh"_a);

        m.def(
            "set_vertex_quality_from_principal_curvature_gaussian",
            [](MeshType& m) {
                return setVertexQualityFromPrincipalCurvatureGaussian(m);
            },
            "mesh"_a);

        m.def(
            "set_vertex_quality_from_principal_curvature_mean",
            [](MeshType& m) {
                return setVertexQualityFromPrincipalCurvatureMean(m);
            },
            "mesh"_a);

        m.def(
            "set_vertex_quality_from_principal_curvature_min_value",
            [](MeshType& m) {
                return setVertexQualityFromPrincipalCurvatureMinValue(m);
            },
            "mesh"_a);

        m.def(
            "set_vertex_quality_from_principal_curvature_max_value",
            [](MeshType& m) {
                return setVertexQualityFromPrincipalCurvatureMaxValue(m);
            },
            "mesh"_a);

        m.def(
            "set_vertex_quality_from_principal_curvature_shape_index",
            [](MeshType& m) {
                return setVertexQualityFromPrincipalCurvatureShapeIndex(m);
            },
            "mesh"_a);

        m.def(
            "set_vertex_quality_from_principal_curvature_curvedness",
            [](MeshType& m) {
                return setVertexQualityFromPrincipalCurvatureCurvedness(m);
            },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind::detail
