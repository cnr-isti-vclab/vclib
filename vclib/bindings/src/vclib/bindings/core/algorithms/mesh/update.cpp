// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/stl.h>

namespace vcl::bind {

template<typename T>
concept FaceEdgeMeshConcept = FaceMeshConcept<T> && EdgeMeshConcept<T>;

void initUpdateAlgorithms(pybind11::module& m)
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

            // color.h

            m.def(
                "set_vertex_color",
                [](MeshType& m, Color c, bool onlySelected = false) {
                    return setVertexColor(m, c, onlySelected);
                },
                "mesh"_a,
                "color"_a         = Color::White,
                "only_selected"_a = false);

            m.def(
                "set_vertex_color_from_quality",
                [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                    return setVertexColorFromQuality(m, cm, minQ, maxQ);
                },
                "mesh"_a,
                "color_map"_a   = Color::ColorMap::RedBlue,
                "min_quality"_a = 0,
                "max_quality"_a = 0);

            m.def(
                "set_vertex_color_from_material",
                [](MeshType& m) { return setVertexColorFromMaterial(m); },
                "mesh"_a);

            m.def(
                "set_vertex_color_perlin_noise",
                [](MeshType& m,
                   Point3d&  period,
                   Point3d&  offset,
                   bool      onlySelected) {
                    return setVertexColorPerlinNoise(
                        m, period, offset, onlySelected);
                },
                "mesh"_a,
                "period"_a,
                "offset"_a        = Point3d(0, 0, 0),
                "only_selected"_a = false);

            m.def(
                "set_vertex_perlin_color",
                [](MeshType& m,
                   double    period,
                   Point3d&  offset,
                   Color&    color1,
                   Color&    color2,
                   bool      onlySelected) {
                    return setVertexPerlinColor(
                        m, period, offset, color1, color2, onlySelected);
                },
                "mesh"_a,
                "period"_a,
                "offset"_a        = Point3d(0, 0, 0),
                "color1"_a        = Color::Black,
                "color2"_a        = Color::White,
                "only_selected"_a = false);

            // normal.h

            m.def(
                "clear_vertex_normals",
                [](MeshType& m) { return clearVertexNormals(m); },
                "mesh"_a);

            m.def(
                "clear_referenced_vertex_normals",
                [](MeshType& m) { return clearReferencedVertexNormals(m); },
                "mesh"_a);

            m.def(
                "normalize_vertex_normals",
                [](MeshType& m) { return normalizeVertexNormals(m); },
                "mesh"_a);

            m.def(
                "normalize_referenced_vertex_normals",
                [](MeshType& m) { return normalizeReferencedVertexNormals(m); },
                "mesh"_a);

            m.def(
                "multiply_vertex_normals_by_matrix",
                [](MeshType&             m,
                   const vcl::Matrix33d& mat,
                   bool                  removeScalingFromMatrix = true,
                   AbstractLogger& log = vcl::nullLogger) {
                    return multiplyVertexNormalsByMatrix(
                        m, mat, removeScalingFromMatrix, log);
                },
                "mesh"_a,
                "matrix"_a,
                "remove_scaling_from_matrix"_a = true,
                "log"_a                        = py::cast(&vcl::nullLogger));

            m.def(
                "multiply_vertex_normals_by_matrix",
                [](MeshType&             m,
                   const vcl::Matrix44d& mat,
                   bool                  removeScalingFromMatrix = true,
                   AbstractLogger& log = vcl::nullLogger) {
                    return multiplyVertexNormalsByMatrix(
                        m, mat, removeScalingFromMatrix, log);
                },
                "mesh"_a,
                "matrix"_a,
                "remove_scaling_from_matrix"_a = true,
                "log"_a                        = py::cast(&vcl::nullLogger));

            // quality.h

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

            // selection.h

            m.def(
                "clear_vertex_selection",
                [](MeshType& m) { return clearVertexSelection(m); },
                "mesh"_a);

            // topology.h

            m.def(
                "clear_vertex_adjacent_vertices",
                [](MeshType& m) { return clearVertexAdjacentVertices(m); },
                "mesh"_a);

            m.def(
                "update_vertex_adjacent_vertices",
                [](MeshType& m) { return updateVertexAdjacentVertices(m); },
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

        m.def(
            "update_border",
            [](MeshType& m) { return vcl::updateBorder(m); },
            "mesh"_a);

        // color.h

        m.def(
            "set_face_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setFaceColor(m, c, onlySelected);
            },
            "mesh"_a,
            "color"_a         = Color::White,
            "only_selected"_a = false);

        m.def(
            "set_vertex_color_from_face_color",
            [](MeshType& m) { return vcl::setVertexColorFromFaceColor(m); },
            "mesh"_a);

        m.def(
            "set_face_color_from_vertex_color",
            [](MeshType& m) { return vcl::setFaceColorFromVertexColor(m); },
            "mesh"_a);

        m.def(
            "set_face_color_from_quality",
            [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                return vcl::setFaceColorFromQuality(m, cm, minQ, maxQ);
            },
            "mesh"_a,
            "color_map"_a   = Color::ColorMap::RedBlue,
            "min_quality"_a = 0,
            "max_quality"_a = 0);

        m.def(
            "set_face_color_from_material",
            [](MeshType& m) { return setFaceColorFromMaterial(m); },
            "mesh"_a);

        m.def(
            "set_vertex_color_from_face_border_flag",
            [](MeshType& m,
               Color     borderColor   = Color::Blue,
               Color     internalColor = Color::White,
               Color     mixColor      = Color::Cyan) {
                return vcl::setVertexColorFromFaceBorderFlag(
                    m, borderColor, internalColor, mixColor);
            },
            "mesh"_a,
            "border_color"_a   = Color::Blue,
            "internal_color"_a = Color::White,
            "mix_color"_a      = Color::Cyan);

        m.def(
            "set_face_color_from_connected_components",
            [](MeshType&                          m,
               const std::vector<std::set<uint>>& connectedComponents) {
                return vcl::setFaceColorFromConnectedComponents(
                    m, connectedComponents);
            },
            "mesh"_a,
            "connected_components"_a);

        m.def(
            "set_face_color_from_connected_components",
            [](MeshType& m) {
                return vcl::setFaceColorFromConnectedComponents(m);
            },
            "mesh"_a);

        m.def(
            "set_face_color_scattering",
            [](MeshType& m, uint nColors = 50, bool checkFauxEdges = true) {
                return vcl::setFaceColorScattering(m, nColors, checkFauxEdges);
            },
            "mesh"_a,
            "n_colors"_a         = 50,
            "check_faux_edges"_a = true);

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

        // normal.h

        m.def(
            "clear_face_normals",
            [](MeshType& m) { return vcl::clearFaceNormals(m); },
            "mesh"_a);

        m.def(
            "normalize_face_normals",
            [](MeshType& m) { return vcl::normalizeFaceNormals(m); },
            "mesh"_a);

        m.def(
            "multiply_face_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix33d& mat,
               bool                  removeScalingFromMatrix = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::multiplyFaceNormalsByMatrix(
                    m, mat, removeScalingFromMatrix, log);
            },
            "mesh"_a,
            "matrix"_a,
            "remove_scaling_from_matrix"_a = true,
            "log"_a                        = py::cast(&vcl::nullLogger));

        m.def(
            "multiply_face_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix44d& mat,
               bool                  removeScalingFromMatrix = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::multiplyFaceNormalsByMatrix(
                    m, mat, removeScalingFromMatrix, log);
            },
            "mesh"_a,
            "matrix"_a,
            "remove_scaling_from_matrix"_a = true,
            "log"_a                        = py::cast(&vcl::nullLogger));

        m.def(
            "update_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateFaceNormals(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormals(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals_from_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormalsFromFaceNormals(
                    m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_and_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexAndFaceNormals(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals_angle_weighted",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormalsAngleWeighted(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals_nelson_max_weighted",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormalsNelsonMaxWeighted(
                    m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

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

        // selection.h

        m.def(
            "clear_face_selection",
            [](MeshType& m) { return clearFaceSelection(m); },
            "mesh"_a);

        m.def(
            "clear_face_edges_selection",
            [](MeshType& m) { return clearFaceEdgesSelection(m); },
            "mesh"_a);

        m.def(
            "select_non_manifold_vertices",
            [](MeshType& m, bool csf) {
                return selectNonManifoldVertices(m, csf);
            },
            "mesh"_a,
            "clear_selection_first"_a = true);

        m.def(
            "select_crease_face_edges",
            [](MeshType& m, double arn, double arp, double abe) {
                return selectCreaseFaceEdges(m, arn, arp, abe);
            },
            "mesh"_a,
            "angle_rad_neg"_a,
            "angle_rad_pos"_a,
            "also_border_edges"_a = false);

        // topology.h

        m.def(
            "clear_vertex_adjacent_faces",
            [](MeshType& m) { return clearVertexAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "update_vertex_adjacent_faces",
            [](MeshType& m) { return updateVertexAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "clear_face_adjacent_faces",
            [](MeshType& m) { return clearFaceAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "update_face_adjacent_faces",
            [](MeshType& m) { return updateFaceAdjacentFaces(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // color.h

        m.def(
            "set_edge_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setEdgeColor(m, c, onlySelected);
            },
            "mesh"_a,
            "color"_a         = Color::White,
            "only_selected"_a = false);

        m.def(
            "set_edge_color_from_vertex_color",
            [](MeshType& m) { return vcl::setEdgeColorFromVertexColor(m); },
            "mesh"_a);

        // selection.h

        m.def(
            "clear_edge_selection",
            [](MeshType& m) { return clearEdgeSelection(m); },
            "mesh"_a);

        // topology.h

        m.def(
            "clear_vertex_adjacent_edges",
            [](MeshType& m) { return clearVertexAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "update_vertex_adjacent_edges",
            [](MeshType& m) { return updateVertexAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "clear_edge_adjacent_edges",
            [](MeshType& m) { return clearEdgeAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "update_edge_adjacent_edges",
            [](MeshType& m) { return updateEdgeAdjacentEdges(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fEdgeMeshes);

    auto fFaceEdgeMeshes = []<FaceEdgeMeshConcept MeshType>(
                               pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "clear_face_adjacent_edges",
            [](MeshType& m) { return clearFaceAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "update_face_adjacent_edges",
            [](MeshType& m) { return updateFaceAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "clear_edge_adjacent_faces",
            [](MeshType& m) { return clearEdgeAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "update_edge_adjacent_faces",
            [](MeshType& m) { return updateEdgeAdjacentFaces(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceEdgeMeshes);
}

} // namespace vcl::bind
