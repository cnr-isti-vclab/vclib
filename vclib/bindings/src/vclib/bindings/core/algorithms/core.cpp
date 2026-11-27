// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/core.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <stdexcept>

namespace vcl::bind {

namespace detail {

// Detects whether a Python array or sequence represents 2D or 3D points.
inline uint inferPointDim(pybind11::handle obj)
{
    namespace py = pybind11;

    if (py::isinstance<py::array>(obj)) {
        auto arr = py::reinterpret_borrow<py::array>(obj);
        if (arr.ndim() == 2 && (arr.shape(1) == 2 || arr.shape(1) == 3)) {
            return static_cast<uint>(arr.shape(1));
        }
        throw std::invalid_argument(
            "Expected a 2D array with shape (N, 2) or (N, 3).");
    }

    if (py::isinstance<py::sequence>(obj) && !py::isinstance<py::str>(obj) &&
        !py::isinstance<py::bytes>(obj)) {
        py::sequence seq = obj.cast<py::sequence>();
        if (seq.empty()) {
            return 2;
        }
        py::handle first = seq[0];
        if (py::isinstance<Point2d>(first))
            return 2;
        if (py::isinstance<Point3d>(first))
            return 3;
        if (py::isinstance<py::buffer>(first)) {
            auto info = first.cast<py::buffer>().request();
            if (info.ndim == 1 && (info.shape[0] == 2 || info.shape[0] == 3)) {
                return static_cast<uint>(info.shape[0]);
            }
        }
        if (py::isinstance<py::sequence>(first) &&
            !py::isinstance<py::str>(first) &&
            !py::isinstance<py::bytes>(first)) {
            std::size_t len = first.cast<py::sequence>().size();
            if (len == 2 || len == 3) {
                return static_cast<uint>(len);
            }
        }
    }

    throw std::invalid_argument(
        "Expected a Polygon2/3 or a sequence/array of 2D or 3D points.");
}

// Registers symmetric intersect(a, b) / intersect(b, a) pair.
template<typename A, typename B>
void defSymmetricIntersect(
    pybind11::module& m,
    const char*       nameA,
    const char*       nameB)
{
    m.def(
        "intersect",
        [](const A& a, const B& b) { return vcl::intersect(a, b); },
        pybind11::arg(nameA),
        pybind11::arg(nameB));

    if constexpr (!std::is_same_v<A, B>) {
        m.def(
            "intersect",
            [](const B& b, const A& a) { return vcl::intersect(b, a); },
            pybind11::arg(nameB),
            pybind11::arg(nameA));
    }
}

// Registers overloads for array, list, and tuple without a catch-all py::object
// that could shadow unrelated typed overloads (such as Face in ear_cut).
template<typename Fn>
void defForPointSeqOrArray(
    pybind11::module& m,
    const char*       name,
    const char*       argName,
    Fn&&              fn)
{
    namespace py = pybind11;
    m.def(name, [fn](const py::array& a) { return fn(a); }, py::arg(argName));
    m.def(name, [fn](const py::list& l) { return fn(l); }, py::arg(argName));
    m.def(name, [fn](const py::tuple& t) { return fn(t); }, py::arg(argName));
}

} // namespace detail

void initCoreAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    // --- Geometric intersection (core/intersection/intersect.h) ---

    detail::defSymmetricIntersect<Planed, Box3d>(m, "plane", "box");
    detail::defSymmetricIntersect<Planed, Segment3d>(m, "plane", "segment");
    detail::defSymmetricIntersect<Sphered, Box3d>(m, "sphere", "box");
    detail::defSymmetricIntersect<Triangle2d, Point2d>(m, "triangle", "point");
    detail::defSymmetricIntersect<Triangle3d, Point3d>(m, "triangle", "point");
    detail::defSymmetricIntersect<Triangle3d, Box3d>(m, "triangle", "box");
    detail::defSymmetricIntersect<Triangle3d, Sphered>(m, "triangle", "sphere");
    detail::defSymmetricIntersect<Ray3d, Triangle3d>(m, "ray", "triangle");

    m.def(
        "intersection",
        [](const Planed& plane, const Segment3d& segment) {
            return vcl::intersection(plane, segment);
        },
        "plane"_a,
        "segment"_a);

    m.def(
        "intersection",
        [](const Segment3d& segment, const Planed& plane) {
            return vcl::intersection(plane, segment);
        },
        "segment"_a,
        "plane"_a);

    m.def(
        "intersection",
        [](const Ray3d& ray, const Triangle3d& triangle) {
            return vcl::intersection(ray, triangle);
        },
        "ray"_a,
        "triangle"_a);

    m.def(
        "intersection",
        [](const Triangle3d& triangle, const Ray3d& ray) {
            return vcl::intersection(ray, triangle);
        },
        "triangle"_a,
        "ray"_a);

    // --- Geometric distance (core/distance/distance.h) ---

    m.def(
        "distance",
        [](const Point2d& p, const Segment2d& s) {
            return vcl::distance(p, s);
        },
        "point"_a,
        "segment"_a);

    m.def(
        "distance",
        [](const Segment2d& s, const Point2d& p) {
            return vcl::distance(s, p);
        },
        "segment"_a,
        "point"_a);

    m.def(
        "distance",
        [](const Point3d& p, const Segment3d& s) {
            return vcl::distance(p, s);
        },
        "point"_a,
        "segment"_a);

    m.def(
        "distance",
        [](const Segment3d& s, const Point3d& p) {
            return vcl::distance(s, p);
        },
        "segment"_a,
        "point"_a);

    m.def(
        "distance",
        [](const Point3d& p, const Triangle3d& t, bool signedDist) {
            return vcl::distance(p, t, signedDist);
        },
        "point"_a,
        "triangle"_a,
        "signed_dist"_a = false);

    m.def(
        "distance",
        [](const Triangle3d& t, const Point3d& p, bool signedDist) {
            return vcl::distance(t, p, signedDist);
        },
        "triangle"_a,
        "point"_a,
        "signed_dist"_a = false);

    m.def(
        "bounded_distance",
        [](const Point3d&    p,
           const Triangle3d& t,
           double            maxDist,
           bool              signedDist) {
            return vcl::boundedDistance(p, t, maxDist, signedDist);
        },
        "point"_a,
        "triangle"_a,
        "max_dist"_a,
        "signed_dist"_a = false);

    m.def(
        "bounded_distance",
        [](const Triangle3d& t,
           const Point3d&    p,
           double            maxDist,
           bool              signedDist) {
            return vcl::boundedDistance(p, t, maxDist, signedDist);
        },
        "triangle"_a,
        "point"_a,
        "max_dist"_a,
        "signed_dist"_a = false);

    // --- Polygon algorithms (core/polygon/*) ---

    m.def(
        "ear_cut",
        [](const Polygon2d& poly) { return vcl::earCut(poly); },
        "polygon"_a);

    m.def(
        "ear_cut",
        [](const Polygon3d& poly) { return vcl::earCut(poly); },
        "polygon"_a);

    detail::defForPointSeqOrArray(m, "ear_cut", "points", [](py::handle pts) {
        uint dim = detail::inferPointDim(pts);
        if (dim == 2) {
            return vcl::earCut(pyToPointVector<2>(pts));
        }
        return vcl::earCut(pyToPointVector<3>(pts));
    });

    m.def(
        "convex_hull",
        [](const Polygon2d& poly) { return vcl::convexHull(poly); },
        "polygon"_a);

    detail::defForPointSeqOrArray(
        m, "convex_hull", "points", [](py::handle points) {
            auto pts = pyToPointVector<2>(points);
            return vcl::convexHull(pts);
        });

    m.def(
        "project_polygon",
        [](const Polygon3d& poly) { return vcl::project(poly); },
        "polygon"_a);

    detail::defForPointSeqOrArray(
        m, "project_polygon", "points", [](py::handle points) {
            auto pts = pyToPointVector<3>(points);
            return vcl::project(pts);
        });

    m.def(
        "are_counter_clockwise",
        [](const Point2d& p0, const Point2d& p1, const Point2d& p2) {
            return vcl::areCounterClockwise(p0, p1, p2);
        },
        "p0"_a,
        "p1"_a,
        "p2"_a);

    m.def(
        "is_counter_clockwise",
        [](const Polygon2d& poly) {
            return vcl::isCounterClockWise(poly.begin(), poly.end());
        },
        "polygon"_a);

    detail::defForPointSeqOrArray(
        m, "is_counter_clockwise", "points", [](py::handle points) {
            auto pts = pyToPointVector<2>(points);
            return vcl::isCounterClockWise(pts.begin(), pts.end());
        });

    m.def(
        "sort_convex_polygon_vertices",
        [](const Polygon2d& poly) {
            Polygon2d sorted = poly;
            if (sorted.size() > 1) {
                vcl::sortConvexPolygonVertices(sorted.begin(), sorted.end());
            }
            return sorted;
        },
        "polygon"_a);

    detail::defForPointSeqOrArray(
        m, "sort_convex_polygon_vertices", "points", [](py::handle points) {
            auto pts = pyToPointVector<2>(points);
            if (pts.size() > 1) {
                vcl::sortConvexPolygonVertices(pts.begin(), pts.end());
            }
            return pts;
        });

    m.def(
        "create_circle",
        [](uint n, double radius) {
            return vcl::createCircle<Polygon2d>(n, radius);
        },
        "n"_a,
        "radius"_a = 1.0);

    // --- Fitting (core/fitting.h) ---

    detail::defForPointSeqOrArray(
        m, "fit_plane_to_point_cloud", "points", [](py::handle points) {
            auto pts = pyToPointVector<3>(points);
            if (pts.empty()) {
                throw std::invalid_argument(
                    "Cannot fit plane to an empty point set.");
            }
            return vcl::fitPlaneToPointCloud<double>(pts);
        });

    auto fitWeightedPlane = [](py::handle                 points,
                               const std::vector<double>& weights) {
        auto pts = pyToPointVector<3>(points);
        if (pts.empty()) {
            throw std::invalid_argument(
                "Cannot fit plane to an empty point set.");
        }
        if (pts.size() != weights.size()) {
            throw WrongSizeException("Weights size must match point count.");
        }
        return vcl::fitPlaneToWeightedPointCloud<Point3d>(pts, weights);
    };
    m.def(
        "fit_plane_to_weighted_point_cloud",
        [fitWeightedPlane](
            const py::array& points, const std::vector<double>& weights) {
            return fitWeightedPlane(points, weights);
        },
        "points"_a,
        "weights"_a);
    m.def(
        "fit_plane_to_weighted_point_cloud",
        [fitWeightedPlane](
            const py::list& points, const std::vector<double>& weights) {
            return fitWeightedPlane(points, weights);
        },
        "points"_a,
        "weights"_a);
    m.def(
        "fit_plane_to_weighted_point_cloud",
        [fitWeightedPlane](
            const py::tuple& points, const std::vector<double>& weights) {
            return fitWeightedPlane(points, weights);
        },
        "points"_a,
        "weights"_a);

    // --- Fibonacci & Perlin Noise ---

    m.def(
        "spherical_fibonacci_point_set",
        [](uint n) { return vcl::sphericalFibonacciPointSet<Point3d>(n); },
        "n"_a);

    m.def(
        "perlin_noise",
        [](double x, double y, double z) { return vcl::perlinNoise(x, y, z); },
        "x"_a,
        "y"_a,
        "z"_a);

    // --- Matrix Camera / Projection (core/matrix_camera.h) ---

    m.def(
        "look_at_matrix",
        [](const Point3d& eye,
           const Point3d& center,
           const Point3d& up,
           Handedness     handedness) {
            return vcl::lookAtMatrix<Matrix44d>(eye, center, up, handedness);
        },
        "eye"_a,
        "center"_a,
        "up"_a,
        "handedness"_a = Handedness::RIGHT_HAND);

    m.def(
        "look_at_matrix_left_handed",
        [](const Point3d& eye, const Point3d& center, const Point3d& up) {
            return vcl::lookAtMatrixLeftHanded<Matrix44d>(eye, center, up);
        },
        "eye"_a,
        "center"_a,
        "up"_a);

    m.def(
        "projection_matrix",
        [](double     fov,
           double     aspect,
           double     nearPlane,
           double     farPlane,
           bool       homogeneousNDC,
           Handedness handedness) {
            return vcl::projectionMatrix<Matrix44d>(
                fov, aspect, nearPlane, farPlane, homogeneousNDC, handedness);
        },
        "fov"_a,
        "aspect"_a,
        "near_plane"_a,
        "far_plane"_a,
        "homogeneous_ndc"_a,
        "handedness"_a = Handedness::RIGHT_HAND);

    m.def(
        "projection_matrix_left_handed",
        [](double fov,
           double aspect,
           double nearPlane,
           double farPlane,
           bool   homogeneousNDC) {
            return vcl::projectionMatrixLeftHanded<Matrix44d>(
                fov, aspect, nearPlane, farPlane, homogeneousNDC);
        },
        "fov"_a,
        "aspect"_a,
        "near_plane"_a,
        "far_plane"_a,
        "homogeneous_ndc"_a);

    m.def(
        "ortho_projection_matrix",
        [](double     left,
           double     right,
           double     top,
           double     bottom,
           double     nearPlane,
           double     farPlane,
           bool       homogeneousNDC,
           Handedness handedness) {
            return vcl::orthoProjectionMatrix<Matrix44d>(
                left,
                right,
                top,
                bottom,
                nearPlane,
                farPlane,
                homogeneousNDC,
                handedness);
        },
        "left"_a,
        "right"_a,
        "top"_a,
        "bottom"_a,
        "near_plane"_a,
        "far_plane"_a,
        "homogeneous_ndc"_a,
        "handedness"_a = Handedness::RIGHT_HAND);

    m.def(
        "project_screen_position",
        [](const Point3d&   pt,
           const Matrix44d& modelViewProjection,
           const Point4d&   viewport,
           bool             homogeneousNDC) {
            return vcl::projectScreenPosition<Matrix44d>(
                pt, modelViewProjection, viewport, homogeneousNDC);
        },
        "point"_a,
        "model_view_projection"_a,
        "viewport"_a,
        "homogeneous_ndc"_a);

    m.def(
        "unproject_screen_position",
        [](const Point3d&   screenPos,
           const Matrix44d& modelViewProjection,
           const Point4d&   viewport,
           bool             homogeneousNDC) {
            return vcl::unprojectScreenPosition<Matrix44d>(
                screenPos, modelViewProjection, viewport, homogeneousNDC);
        },
        "screen_pos"_a,
        "model_view_projection"_a,
        "viewport"_a,
        "homogeneous_ndc"_a);

    // --- Visibility (core/visibility.h) ---

    m.def(
        "half_space_determinant",
        [](const Triangle3d& triangle, const Point3d& point) {
            return vcl::halfSpaceDeterminant(triangle, point);
        },
        "triangle"_a,
        "point"_a);

    m.def(
        "half_space_determinant",
        [](const Point3d& p0,
           const Point3d& p1,
           const Point3d& p2,
           const Point3d& p) {
            return vcl::halfSpaceDeterminant(p0, p1, p2, p);
        },
        "p0"_a,
        "p1"_a,
        "p2"_a,
        "point"_a);

    m.def(
        "triangle_point_visibility",
        [](const Triangle3d& triangle, const Point3d& point) {
            return vcl::trianglePointVisibility(triangle, point);
        },
        "triangle"_a,
        "point"_a);

    m.def(
        "triangle_point_visibility",
        [](const Point3d& p0,
           const Point3d& p1,
           const Point3d& p2,
           const Point3d& p) {
            return vcl::trianglePointVisibility(p0, p1, p2, p);
        },
        "p0"_a,
        "p1"_a,
        "p2"_a,
        "point"_a);

    // --- Random Barycentric & Create (core/random.h, core/create.h) ---

    m.def(
        "random_triangle_barycentric_coordinate",
        [](std::optional<uint> seed) {
            return vcl::randomTriangleBarycentricCoordinate<Point3d>(
                toRConfig(seed));
        },
        "seed"_a = py::none());

    m.def(
        "random_polygon_barycentric_coordinate",
        [](uint polySize, std::optional<uint> seed) {
            if (polySize == 0) {
                throw std::invalid_argument("poly_size must be > 0.");
            }
            return vcl::randomPolygonBarycentricCoordinate<double>(
                polySize, toRConfig(seed));
        },
        "poly_size"_a,
        "seed"_a = py::none());

    m.def(
        "create_check_board_image",
        [](uint imageSize, uint checkNum) {
            if (checkNum == 0) {
                throw std::invalid_argument("check_num must be > 0.");
            }
            return vcl::createCheckBoardImage(imageSize, checkNum);
        },
        "image_size"_a,
        "check_num"_a = 8);
}

} // namespace vcl::bind
