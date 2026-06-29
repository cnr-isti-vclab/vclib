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

template<typename ScalarType>
class PointCloudT;

} // namespace vcl

namespace vcl::pointcloud {

template<typename Scalar>
class Vertex;

/**
 * @brief The Vertex type used by the PointCloudT class.
 *
 * @extends vert::BitFlags
 * @extends vert::Position3
 * @extends vert::Normal3
 * @extends vert::OptionalColor
 * @extends vert::OptionalQuality
 * @extends vert::OptionalAdjacentVertexPointers
 * @extends vert::OptionalTexCoord
 * @extends vert::OptionalMaterialIndex
 * @extends vert::OptionalMark
 * @extends vert::CustomComponents
 *
 * @tparam Scalar: The scalar type used for the mesh.
 *
 * @ingroup meshes
 */
template<typename Scalar>
class Vertex :
        public vcl::Vertex<
            PointCloudT<Scalar>,
            vert::BitFlags,
            vert::Position3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vert::OptionalAdjacentVertexPointers<Vertex<Scalar>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vert::OptionalMaterialIndex<Vertex<Scalar>>,
            vert::OptionalMark<Vertex<Scalar>>,
            vert::CustomComponents<Vertex<Scalar>>>
{
public:
    friend void swap(Vertex& a, Vertex& b) { a.swap(b); }
};

} // namespace vcl::pointcloud

namespace vcl {

/**
 * @brief The PointCloudT class is a mesh class that represents a point cloud.
 *
 * It allows to store only pointcloud::Vertex elements.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 *
 * @extends mesh::VertexContainer
 * @extends mesh::BoundingBox3
 * @extends mesh::Mark
 * @extends mesh::Materials
 * @extends mesh::Name
 * @extends mesh::TransformMatrix
 * @extends mesh::CustomComponents
 *
 * @ingroup meshes
 */
template<typename Scalar>
class PointCloudT :
        public Mesh<
            mesh::VertexContainer<pointcloud::Vertex<Scalar>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Mark,
            mesh::Materials,
            mesh::Name,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    /** @brief The scalar used to store all the data of the Mesh. */
    using ScalarType = Scalar;
};

/**
 * @brief The PointCloudf class is a specialization of the PointCloudT class
 * that uses `float` as scalar.
 * @ingroup meshes
 */
using PointCloudf = PointCloudT<float>;

/**
 * @brief The PointCloud class is a specialization of the PointCloudT class
 * that uses `double` as scalar.
 * @ingroup meshes
 */
using PointCloud = PointCloudT<double>;

} // namespace vcl

#endif // VCL_MESHES_POINT_CLOUD_H
