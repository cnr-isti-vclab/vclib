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

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initUpdateAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            // bounding_box.h

            m.def("update_bounding_box", [](MeshType& m) {
                return updateBoundingBox(m);
            });

            // color.h

            m.def(
                "set_per_vertex_color",
                [](MeshType& m, Color c, bool onlySelected = false) {
                    return setPerVertexColor(m, c, onlySelected);
                },
                py::arg("mesh"),
                py::arg("color")         = Color::White,
                py::arg("only_selected") = false);

            m.def(
                "set_per_vertex_color_from_quality",
                [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                    return setPerVertexColorFromQuality(m, cm, minQ, maxQ);
                },
                py::arg("mesh"),
                py::arg("color_map")   = Color::ColorMap::RedBlue,
                py::arg("min_quality") = 0,
                py::arg("max_quality") = 0);

            m.def(
                "set_per_vertex_color_perlin_noise",
                [](MeshType& m,
                   Point3d&  period,
                   Point3d&  offset,
                   bool      onlySelected) {
                    return setPerVertexColorPerlinNoise(
                        m, period, offset, onlySelected);
                },
                py::arg("mesh"),
                py::arg("period"),
                py::arg("offset")        = Point3d(0, 0, 0),
                py::arg("only_selected") = false);

            m.def(
                "set_per_vertex_perlin_color",
                [](MeshType& m,
                   double    period,
                   Point3d&  offset,
                   Color&    color1,
                   Color&    color2,
                   bool      onlySelected) {
                    return setPerVertexPerlinColor(
                        m, period, offset, color1, color2, onlySelected);
                },
                py::arg("mesh"),
                py::arg("period"),
                py::arg("offset")        = Point3d(0, 0, 0),
                py::arg("color1")        = Color::Black,
                py::arg("color2")        = Color::White,
                py::arg("only_selected") = false);

            // normal.h

            m.def("clear_per_vertex_normals", [](MeshType& m) {
                return clearPerVertexNormals(m);
            });

            m.def("clear_per_referenced_vertex_normals", [](MeshType& m) {
                return clearPerReferencedVertexNormals(m);
            });

            m.def("normalize_per_vertex_normals", [](MeshType& m) {
                return normalizePerVertexNormals(m);
            });

            m.def("normalize_per_referenced_vertex_normals", [](MeshType& m) {
                return normalizePerReferencedVertexNormals(m);
            });

            m.def(
                "multiply_per_vertex_normals_by_matrix",
                [](MeshType&             m,
                   const vcl::Matrix33d& mat,
                   bool                  removeScalingFromMatrix = true,
                   AbstractLogger& log = vcl::nullLogger) {
                    return multiplyPerVertexNormalsByMatrix(
                        m, mat, removeScalingFromMatrix, log);
                },
                py::arg("mesh"),
                py::arg("matrix"),
                py::arg("remove_scaling_from_matrix") = true,
                py::arg("log") = py::cast(vcl::nullLogger));

            m.def(
                "multiply_per_vertex_normals_by_matrix",
                [](MeshType&             m,
                   const vcl::Matrix44d& mat,
                   bool                  removeScalingFromMatrix = true,
                   AbstractLogger& log = vcl::nullLogger) {
                    return multiplyPerVertexNormalsByMatrix(
                        m, mat, removeScalingFromMatrix, log);
                },
                py::arg("mesh"),
                py::arg("matrix"),
                py::arg("remove_scaling_from_matrix") = true,
                py::arg("log") = py::cast(vcl::nullLogger));

            // quality.h

            m.def(
                "set_per_vertex_quality",
                [](MeshType& m, double quality) {
                    return setPerVertexQuality(m, quality);
                },
                py::arg("mesh"),
                py::arg("quality") = 0.0);

            m.def(
                "clamp_per_vertex_quality",
                [](MeshType& m, double minQ, double maxQ) {
                    return clampPerVertexQuality(m, minQ, maxQ);
                },
                py::arg("mesh"),
                py::arg("min_quality") = 0.0,
                py::arg("max_quality") = 1.0);

            m.def(
                "normalize_per_vertex_quality",
                [](MeshType& m, double minQ, double maxQ) {
                    return normalizePerVertexQuality(m, minQ, maxQ);
                },
                py::arg("mesh"),
                py::arg("min_quality") = 0.0,
                py::arg("max_quality") = 1.0);

            // selection.h

            m.def("clear_vertex_selection", [](MeshType& m) {
                return clearVertexSelection(m);
            });
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
        // border.h

        m.def("update_border", [](MeshType& m) {
            return vcl::updateBorder(m);
        });

        // color.h

        m.def(
            "set_per_face_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setPerFaceColor(m, c, onlySelected);
            },
            py::arg("mesh"),
            py::arg("color")         = Color::White,
            py::arg("only_selected") = false);

        m.def("set_per_vertex_color_from_face_color", [](MeshType& m) {
            return vcl::setPerVertexColorFromFaceColor(m);
        });

        m.def("set_per_face_color_from_vertex_color", [](MeshType& m) {
            return vcl::setPerFaceColorFromVertexColor(m);
        });

        m.def(
            "set_per_face_color_from_quality",
            [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                return vcl::setPerFaceColorFromQuality(m, cm, minQ, maxQ);
            },
            py::arg("mesh"),
            py::arg("color_map")   = Color::ColorMap::RedBlue,
            py::arg("min_quality") = 0,
            py::arg("max_quality") = 0);

        m.def(
            "set_per_vertex_color_from_face_border_flag",
            [](MeshType& m,
               Color     borderColor   = Color::Blue,
               Color     internalColor = Color::White,
               Color     mixColor      = Color::Cyan) {
                return vcl::setPerVertexColorFromFaceBorderFlag(
                    m, borderColor, internalColor, mixColor);
            },
            py::arg("mesh"),
            py::arg("border_color")   = Color::Blue,
            py::arg("internal_color") = Color::White,
            py::arg("mix_color")      = Color::Cyan);

        m.def(
            "set_per_face_color_from_connected_components",
            [](MeshType&                          m,
               const std::vector<std::set<uint>>& connectedComponents) {
                return vcl::setPerFaceColorFromConnectedComponents(
                    m, connectedComponents);
            },
            py::arg("mesh"),
            py::arg("connected_components"));

        m.def(
            "set_per_face_color_from_connected_components",
            [](MeshType& m) {
                return vcl::setPerFaceColorFromConnectedComponents(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_face_color_scattering",
            [](MeshType& m, uint nColors = 50, bool checkFauxEdges = true) {
                return vcl::setPerFaceColorScattering(
                    m, nColors, checkFauxEdges);
            },
            py::arg("mesh"),
            py::arg("n_colors")         = 50,
            py::arg("check_faux_edges") = true);

        // curvature.h

        m.def(
            "update_per_vertex_principal_curvature",
            [](MeshType&                   m,
               PrincipalCurvatureAlgorithm alg =
                   PrincipalCurvatureAlgorithm::TAUBIN95,
               AbstractLogger& log = nullLogger) {
                return vcl::updatePrincipalCurvature(m, alg, log);
            },
            py::arg("mesh"),
            py::arg("algorithm") = PrincipalCurvatureAlgorithm::TAUBIN95,
            py::arg("log")       = py::cast(vcl::nullLogger));

        // normal.h

        m.def("clear_per_face_normals", [](MeshType& m) {
            return vcl::clearPerFaceNormals(m);
        });

        m.def("normalize_per_face_normals", [](MeshType& m) {
            return vcl::normalizePerFaceNormals(m);
        });

        m.def(
            "multiply_per_face_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix33d& mat,
               bool                  removeScalingFromMatrix = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::multiplyPerFaceNormalsByMatrix(
                    m, mat, removeScalingFromMatrix, log);
            },
            py::arg("mesh"),
            py::arg("matrix"),
            py::arg("remove_scaling_from_matrix") = true,
            py::arg("log")                        = py::cast(vcl::nullLogger));

        m.def(
            "multiply_per_face_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix44d& mat,
               bool                  removeScalingFromMatrix = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::multiplyPerFaceNormalsByMatrix(
                    m, mat, removeScalingFromMatrix, log);
            },
            py::arg("mesh"),
            py::arg("matrix"),
            py::arg("remove_scaling_from_matrix") = true,
            py::arg("log")                        = py::cast(vcl::nullLogger));

        m.def(
            "update_per_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updatePerFaceNormals(m, normalize, log);
            },
            py::arg("mesh"),
            py::arg("normalize") = true,
            py::arg("log")       = py::cast(vcl::nullLogger));

        m.def(
            "update_per_vertex_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updatePerVertexNormals(m, normalize, log);
            },
            py::arg("mesh"),
            py::arg("normalize") = true,
            py::arg("log")       = py::cast(vcl::nullLogger));

        m.def(
            "update_per_vertex_normals_from_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updatePerVertexNormalsFromFaceNormals(
                    m, normalize, log);
            },
            py::arg("mesh"),
            py::arg("normalize") = true,
            py::arg("log")       = py::cast(vcl::nullLogger));

        m.def(
            "update_per_vertex_and_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updatePerVertexAndFaceNormals(m, normalize, log);
            },
            py::arg("mesh"),
            py::arg("normalize") = true,
            py::arg("log")       = py::cast(vcl::nullLogger));

        m.def(
            "update_per_vertex_normals_angle_weighted",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updatePerVertexNormalsAngleWeighted(
                    m, normalize, log);
            },
            py::arg("mesh"),
            py::arg("normalize") = true,
            py::arg("log")       = py::cast(vcl::nullLogger));

        m.def(
            "update_per_vertex_normals_nelson_max_weighted",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updatePerVertexNormalsNelsonMaxWeighted(
                    m, normalize, log);
            },
            py::arg("mesh"),
            py::arg("normalize") = true,
            py::arg("log")       = py::cast(vcl::nullLogger));

        // quality.h

        m.def(
            "set_per_face_quality",
            [](MeshType& m, double quality) {
                return vcl::setPerFaceQuality(m, quality);
            },
            py::arg("mesh"),
            py::arg("quality") = 0.0);

        m.def(
            "clamp_per_face_quality",
            [](MeshType& m, double minQ, double maxQ) {
                return vcl::clampPerFaceQuality(m, minQ, maxQ);
            },
            py::arg("mesh"),
            py::arg("min_quality") = 0.0,
            py::arg("max_quality") = 1.0);

        m.def(
            "normalize_per_face_quality",
            [](MeshType& m, double minQ, double maxQ) {
                return vcl::normalizePerFaceQuality(m, minQ, maxQ);
            },
            py::arg("mesh"),
            py::arg("min_quality") = 0.0,
            py::arg("max_quality") = 1.0);

        m.def("set_per_vertex_quality_from_vertex_valence", [](MeshType& m) {
            return vcl::setPerVertexQualityFromVertexValence(m);
        });

        m.def("set_per_face_quality_from_face_area", [](MeshType& m) {
            return vcl::setPerFaceQualityFromFaceArea(m);
        });

        m.def(
            "set_per_vertex_quality_from_principal_curvature_gaussian",
            [](MeshType& m) {
                return setPerVertexQualityFromPrincipalCurvatureGaussian(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_vertex_quality_from_principal_curvature_mean",
            [](MeshType& m) {
                return setPerVertexQualityFromPrincipalCurvatureMean(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_vertex_quality_from_principal_curvature_min_value",
            [](MeshType& m) {
                return setPerVertexQualityFromPrincipalCurvatureMinValue(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_vertex_quality_from_principal_curvature_max_value",
            [](MeshType& m) {
                return setPerVertexQualityFromPrincipalCurvatureMaxValue(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_vertex_quality_from_principal_curvature_shape_index",
            [](MeshType& m) {
                return setPerVertexQualityFromPrincipalCurvatureShapeIndex(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_vertex_quality_from_principal_curvature_curvedness",
            [](MeshType& m) {
                return setPerVertexQualityFromPrincipalCurvatureCurvedness(m);
            },
            py::arg("mesh"));

        // selection.h

        m.def("clear_face_selection", [](MeshType& m) {
            return clearFaceSelection(m);
        });

        m.def("clear_face_edges_selection", [](MeshType& m) {
            return clearFaceEdgesSelection(m);
        });

        m.def(
            "select_non_manifold_vertices",
            [](MeshType& m, bool csf) {
                return selectNonManifoldVertices(m, csf);
            },
            py::arg("mesh"),
            py::arg("clear_selection_first") = true);

        m.def(
            "select_crease_face_edges",
            [](MeshType& m, double arn, double arp, double abe) {
                return selectCreaseFaceEdges(m, arn, arp, abe);
            },
            py::arg("mesh"),
            py::arg("angle_rad_neg"),
            py::arg("angle_rad_pos"),
            py::arg("also_border_edges") = false);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // color.h

        m.def(
            "set_per_edge_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setPerEdgeColor(m, c, onlySelected);
            },
            py::arg("mesh"),
            py::arg("color")         = Color::White,
            py::arg("only_selected") = false);

        m.def("set_per_edge_color_from_vertex_color", [](MeshType& m) {
            return vcl::setPerEdgeColorFromVertexColor(m);
        });

        // selection.h

        m.def("clear_edge_selection", [](MeshType& m) {
            return clearEdgeSelection(m);
        });
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
