// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/complex/kd_tree.h>
#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/complex.h>

#include <pybind11/stl.h>

#include <stdexcept>
#include <utility>

namespace vcl::bind {

template<uint DIM>
void populateKDTree(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar    = double;
    using PointType = Point<Scalar, DIM>;
    using KT        = KDTree<PointType>;

    std::string    cName = "KDTree" + std::to_string(DIM);
    py::class_<KT> c(m, cName.c_str());

    // Register typed Mesh constructors before the catch-all py::object points
    // overload so pybind11 matches meshes first.
    if constexpr (DIM == 3) {
        auto fAllMeshes = [&cName]<MeshConcept MeshType>(
                              py::class_<KT>& c, MeshType = MeshType()) {
            c.def(
                py::init([cName](
                             const MeshType& mesh,
                             uint            pointsPerCell,
                             uint            maxDepth,
                             bool            balanced) {
                    if (mesh.vertexCount() == 0) {
                        throw std::invalid_argument(
                            "Cannot construct " + cName +
                            " from an empty mesh.");
                    }
                    return KT(mesh, pointsPerCell, maxDepth, balanced);
                }),
                "mesh"_a,
                "points_per_cell"_a = 16,
                "max_depth"_a       = 64,
                "balanced"_a        = false);
        };
        defForAllMeshTypes(c, fAllMeshes);
    }

    auto makeKDTree = [cName](
                          py::handle points,
                          uint       pointsPerCell,
                          uint       maxDepth,
                          bool       balanced) {
        auto pts = pyToPointVector<DIM>(points);
        if (pts.empty()) {
            throw std::invalid_argument(
                "Cannot construct " + cName + " from an empty point set.");
        }
        return KT(pts, pointsPerCell, maxDepth, balanced);
    };

    c.def(
        py::init([makeKDTree](
                     const py::array& points,
                     uint             pointsPerCell,
                     uint             maxDepth,
                     bool             balanced) {
            return makeKDTree(points, pointsPerCell, maxDepth, balanced);
        }),
        "points"_a,
        "points_per_cell"_a = 16,
        "max_depth"_a       = 64,
        "balanced"_a        = false);

    c.def(
        py::init([makeKDTree](
                     const py::list& points,
                     uint            pointsPerCell,
                     uint            maxDepth,
                     bool            balanced) {
            return makeKDTree(points, pointsPerCell, maxDepth, balanced);
        }),
        "points"_a,
        "points_per_cell"_a = 16,
        "max_depth"_a       = 64,
        "balanced"_a        = false);

    c.def(
        py::init([makeKDTree](
                     const py::tuple& points,
                     uint             pointsPerCell,
                     uint             maxDepth,
                     bool             balanced) {
            return makeKDTree(points, pointsPerCell, maxDepth, balanced);
        }),
        "points"_a,
        "points_per_cell"_a = 16,
        "max_depth"_a       = 64,
        "balanced"_a        = false);

    c.def(
        "nearest_neighbor_index",
        [](const KT& kt, const PointType& queryPoint) {
            Scalar dist = 0;
            return kt.nearestNeighborIndex(queryPoint, dist);
        },
        "query_point"_a);

    c.def(
        "nearest_neighbor_index_with_distance",
        [](const KT& kt, const PointType& queryPoint) {
            Scalar      dist = 0;
            std::size_t idx  = kt.nearestNeighborIndex(queryPoint, dist);
            return std::make_pair(idx, dist);
        },
        "query_point"_a);

    c.def(
        "nearest_neighbor",
        [](const KT& kt, const PointType& queryPoint) {
            Scalar dist = 0;
            return kt.nearestNeighbor(queryPoint, dist);
        },
        "query_point"_a);

    c.def(
        "nearest_neighbor_with_distance",
        [](const KT& kt, const PointType& queryPoint) {
            Scalar    dist = 0;
            PointType pt   = kt.nearestNeighbor(queryPoint, dist);
            return std::make_pair(pt, dist);
        },
        "query_point"_a);

    c.def(
        "k_nearest_neighbors_indices",
        [](const KT& kt, const PointType& queryPoint, uint k) {
            if (k == 0) {
                return std::vector<std::size_t> {};
            }
            std::vector<Scalar> dists;
            return kt.kNearestNeighborsIndices(queryPoint, k, dists);
        },
        "query_point"_a,
        "k"_a);

    c.def(
        "k_nearest_neighbors_indices_with_distances",
        [](const KT& kt, const PointType& queryPoint, uint k) {
            std::vector<Scalar> dists;
            if (k == 0) {
                return std::make_pair(std::vector<std::size_t> {}, dists);
            }
            auto indices = kt.kNearestNeighborsIndices(queryPoint, k, dists);
            return std::make_pair(std::move(indices), std::move(dists));
        },
        "query_point"_a,
        "k"_a);

    c.def(
        "k_nearest_neighbors",
        [](const KT& kt, const PointType& queryPoint, uint k) {
            if (k == 0) {
                return std::vector<PointType> {};
            }
            std::vector<Scalar> dists;
            return kt.kNearestNeighbors(queryPoint, k, dists);
        },
        "query_point"_a,
        "k"_a);

    c.def(
        "k_nearest_neighbors_with_distances",
        [](const KT& kt, const PointType& queryPoint, uint k) {
            std::vector<Scalar> dists;
            if (k == 0) {
                return std::make_pair(std::vector<PointType> {}, dists);
            }
            auto pts = kt.kNearestNeighbors(queryPoint, k, dists);
            return std::make_pair(std::move(pts), std::move(dists));
        },
        "query_point"_a,
        "k"_a);

    c.def(
        "neighbors_indices_in_distance",
        [](const KT& kt, const PointType& queryPoint, Scalar dist) {
            std::vector<Scalar> dists;
            return kt.neighborsIndicesInDistance(queryPoint, dist, dists);
        },
        "query_point"_a,
        "dist"_a);

    c.def(
        "neighbors_indices_in_distance_with_distances",
        [](const KT& kt, const PointType& queryPoint, Scalar dist) {
            std::vector<Scalar> dists;
            auto                indices =
                kt.neighborsIndicesInDistance(queryPoint, dist, dists);
            return std::make_pair(std::move(indices), std::move(dists));
        },
        "query_point"_a,
        "dist"_a);

    c.def(
        "neighbors_in_distance",
        [](const KT& kt, const PointType& queryPoint, Scalar dist) {
            std::vector<Scalar> dists;
            return kt.neighborsInDistance(queryPoint, dist, dists);
        },
        "query_point"_a,
        "dist"_a);

    c.def(
        "neighbors_in_distance_with_distances",
        [](const KT& kt, const PointType& queryPoint, Scalar dist) {
            std::vector<Scalar> dists;
            auto pts = kt.neighborsInDistance(queryPoint, dist, dists);
            return std::make_pair(std::move(pts), std::move(dists));
        },
        "query_point"_a,
        "dist"_a);
}

void initKDTree(pybind11::module& m)
{
    populateKDTree<2>(m);
    populateKDTree<3>(m);
}

} // namespace vcl::bind
