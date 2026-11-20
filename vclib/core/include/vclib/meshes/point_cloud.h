// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_POINT_CLOUD_H
#define VCL_MESHES_POINT_CLOUD_H

#include <vclib/mesh.h>

namespace vcl {

namespace pointcloud {

/**
 * @brief The VertexComponents type used by the PointCloudT class.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using VertexComponents = vcl::VertexComponents<
    vert::BitFlags,
    vert::Position3<Scalar>,
    vert::Normal3<Scalar>,
    vert::OptionalColor,
    vert::OptionalQuality<Scalar>,
    vert::OptionalAdjacentVertices<Index>,
    vert::OptionalTexCoord<Scalar>,
    vert::OptionalMaterialIndex,
    vert::CustomComponents>;

} // namespace pointcloud

/**
 * @brief The PointCloudT class is a mesh class that represents a point cloud.
 *
 * It allows to store only pointcloud vertices.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam Index: The unsigned integral type used for indexing elements.
 *
 * @ingroup meshes
 */
template<typename Scalar = double, std::unsigned_integral Index = uint>
using PointCloudT = Mesh<
    mesh::VertexContainer<pointcloud::VertexComponents<Scalar, Index>, Index>,
    mesh::BoundingBox3<Scalar>,
    mesh::Materials,
    mesh::Name,
    mesh::TransformMatrix<Scalar>,
    mesh::CustomComponents>;

/**
 * @brief The PointCloud class is a specialization of the PointCloudT class
 * that uses `double` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using PointCloud = PointCloudT<double, uint>;

/**
 * @brief The PointCloudf class is a specialization of the PointCloudT class
 * that uses `float` as scalar and `uint` for indexing.
 * @ingroup meshes
 */
using PointCloudf = PointCloudT<float, uint>;

/**
 * @brief The PointCloud64 class is a specialization of the PointCloudT class
 * that uses `double` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using PointCloud64 = PointCloudT<double, uint64_t>;

/**
 * @brief The PointCloud64f class is a specialization of the PointCloudT class
 * that uses `float` as scalar and `uint64_t` for indexing.
 * @ingroup meshes
 */
using PointCloud64f = PointCloudT<float, uint64_t>;

} // namespace vcl

#endif // VCL_MESHES_POINT_CLOUD_H
