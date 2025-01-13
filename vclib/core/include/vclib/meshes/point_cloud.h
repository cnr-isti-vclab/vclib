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

#ifndef VCL_MESHES_POINT_CLOUD_H
#define VCL_MESHES_POINT_CLOUD_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename ScalarType>
class PointCloudT;

} // namespace vcl

namespace vcl::pointcloud {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Vertex :
        public vcl::Vertex<
            PointCloudT<Scalar>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vert::OptionalMark<Vertex<Scalar>>,
            vert::CustomComponents<Vertex<Scalar>>>
{
};

} // namespace vcl::pointcloud

namespace vcl {

/**
 * @brief The PointCloudT is a Mesh type that stores only Vertices as elements.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 *
 * @ingroup meshes
 */
template<typename Scalar>
class PointCloudT :
        public Mesh<
            mesh::VertexContainer<pointcloud::Vertex<Scalar>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Mark,
            mesh::Name,
            mesh::TextureImages,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    using ScalarType = Scalar;
};

using PointCloudf = PointCloudT<float>;
using PointCloud  = PointCloudT<double>;

} // namespace vcl

#endif // VCL_MESHES_POINT_CLOUD_H
