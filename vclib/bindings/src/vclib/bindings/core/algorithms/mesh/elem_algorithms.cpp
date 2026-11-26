// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/elem_algorithms.h>
#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/stl.h>

#include <stdexcept>

namespace vcl::bind {

namespace detail {

template<ElementConcept ElemType>
void checkValidElementProxy(const ElemType& el)
{
    if (!el.isValid()) {
        throw std::runtime_error(
            "Cannot run element algorithm on an invalid or null element "
            "proxy.");
    }
}

} // namespace detail

void initElemAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        using VertexType = vcl::VertexType<MeshType>;

        m.def(
            "bounding_box",
            [](const VertexType& v) {
                detail::checkValidElementProxy(v);
                return vcl::boundingBox(v);
            },
            "vertex"_a);

        m.def(
            "distance",
            [](const VertexType& v, const Point3d& p) {
                detail::checkValidElementProxy(v);
                return vcl::distance(v, p);
            },
            "vertex"_a,
            "point"_a);

        m.def(
            "distance",
            [](const Point3d& p, const VertexType& v) {
                detail::checkValidElementProxy(v);
                return vcl::distance(p, v);
            },
            "point"_a,
            "vertex"_a);

        m.def(
            "distance",
            [](const VertexType& v1, const VertexType& v2) {
                detail::checkValidElementProxy(v1);
                detail::checkValidElementProxy(v2);
                return vcl::distance(v1, v2);
            },
            "v1"_a,
            "v2"_a);
    };
    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        using VertexType = vcl::VertexType<MeshType>;
        using FaceType   = vcl::FaceType<MeshType>;

        // bounding_box.h
        m.def(
            "bounding_box",
            [](const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::boundingBox(f);
            },
            "face"_a);

        // polygon/geometry.h & polygon/ear_cut.h
        m.def(
            "face_normal",
            [](const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::faceNormal(f);
            },
            "face"_a);

        m.def(
            "face_barycenter",
            [](const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::faceBarycenter(f);
            },
            "face"_a);

        m.def(
            "face_area",
            [](const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::faceArea(f);
            },
            "face"_a);

        m.def(
            "face_perimeter",
            [](const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::facePerimeter(f);
            },
            "face"_a);

        m.def(
            "face_angle_on_vertex_rad",
            [](const FaceType& f, uint vi) {
                detail::checkValidElementProxy(f);
                if (vi >= f.vertexCount()) {
                    throw py::index_error();
                }
                return vcl::faceAngleOnVertexRad(f, vi);
            },
            "face"_a,
            "vi"_a);

        m.def(
            "ear_cut",
            [](const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::earCut(f);
            },
            "face"_a);

        // visibility.h
        m.def(
            "half_space_determinant",
            [](const FaceType& f, const Point3d& p) {
                detail::checkValidElementProxy(f);
                return vcl::halfSpaceDeterminant(f, p);
            },
            "face"_a,
            "point"_a);

        m.def(
            "face_point_visibility",
            [](const FaceType& f, const Point3d& p) {
                detail::checkValidElementProxy(f);
                return vcl::facePointVisibility(f, p);
            },
            "face"_a,
            "point"_a);

        m.def(
            "face_point_visibility",
            [](const Point3d& p, const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::facePointVisibility(p, f);
            },
            "point"_a,
            "face"_a);

        // distance.h
        m.def(
            "distance",
            [](const Point3d& p, const FaceType& f, bool signedDist) {
                detail::checkValidElementProxy(f);
                return vcl::distance(p, f, signedDist);
            },
            "point"_a,
            "face"_a,
            "signed_dist"_a = false);

        m.def(
            "distance",
            [](const FaceType& f, const Point3d& p, bool signedDist) {
                detail::checkValidElementProxy(f);
                return vcl::distance(f, p, signedDist);
            },
            "face"_a,
            "point"_a,
            "signed_dist"_a = false);

        m.def(
            "distance",
            [](const VertexType& v, const FaceType& f, bool signedDist) {
                detail::checkValidElementProxy(v);
                detail::checkValidElementProxy(f);
                return vcl::distance(v, f, signedDist);
            },
            "vertex"_a,
            "face"_a,
            "signed_dist"_a = false);

        m.def(
            "distance",
            [](const FaceType& f, const VertexType& v, bool signedDist) {
                detail::checkValidElementProxy(f);
                detail::checkValidElementProxy(v);
                return vcl::distance(f, v, signedDist);
            },
            "face"_a,
            "vertex"_a,
            "signed_dist"_a = false);

        m.def(
            "bounded_distance",
            [](const Point3d&  p,
               const FaceType& f,
               double          maxDist,
               bool            signedDist) {
                detail::checkValidElementProxy(f);
                return vcl::boundedDistance(p, f, maxDist, signedDist);
            },
            "point"_a,
            "face"_a,
            "max_dist"_a,
            "signed_dist"_a = false);

        m.def(
            "bounded_distance",
            [](const FaceType& f,
               const Point3d&  p,
               double          maxDist,
               bool            signedDist) {
                detail::checkValidElementProxy(f);
                return vcl::boundedDistance(f, p, maxDist, signedDist);
            },
            "face"_a,
            "point"_a,
            "max_dist"_a,
            "signed_dist"_a = false);

        // intersection.h
        m.def(
            "intersect",
            [](const FaceType& f, const Box3d& box) {
                detail::checkValidElementProxy(f);
                return vcl::intersect(f, box);
            },
            "face"_a,
            "box"_a);

        m.def(
            "intersect",
            [](const Box3d& box, const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::intersect(box, f);
            },
            "box"_a,
            "face"_a);

        m.def(
            "intersect",
            [](const FaceType& f, const Sphered& sphere) {
                detail::checkValidElementProxy(f);
                return vcl::intersect(f, sphere);
            },
            "face"_a,
            "sphere"_a);

        m.def(
            "intersect",
            [](const Sphered& sphere, const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::intersect(sphere, f);
            },
            "sphere"_a,
            "face"_a);

        m.def(
            "intersect",
            [](const Ray3d& ray, const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::intersect(ray, f);
            },
            "ray"_a,
            "face"_a);

        m.def(
            "intersect",
            [](const FaceType& f, const Ray3d& ray) {
                detail::checkValidElementProxy(f);
                return vcl::intersect(f, ray);
            },
            "face"_a,
            "ray"_a);

        m.def(
            "intersection",
            [](const Ray3d& ray, const FaceType& f) {
                detail::checkValidElementProxy(f);
                return vcl::intersection(ray, f);
            },
            "ray"_a,
            "face"_a);

        m.def(
            "intersection",
            [](const FaceType& f, const Ray3d& ray) {
                detail::checkValidElementProxy(f);
                return vcl::intersection(f, ray);
            },
            "face"_a,
            "ray"_a);
    };
    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        using EdgeType = vcl::EdgeType<MeshType>;

        m.def(
            "bounding_box",
            [](const EdgeType& e) {
                detail::checkValidElementProxy(e);
                return vcl::boundingBox(e);
            },
            "edge"_a);
    };
    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
