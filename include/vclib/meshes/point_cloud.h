/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
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
            vcl::vert::BitFlags,
            vcl::vert::Coordinate3<Scalar>,
            vcl::vert::Normal3<Scalar>,
            vcl::vert::Color,
            vcl::vert::Quality<Scalar>,
            vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalMark<Vertex<Scalar>>,
            vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

} // namespace vcl::pointcloud

namespace vcl {

template<typename Scalar = double>
class PointCloudT :
        public vcl::Mesh<
            mesh::VertexContainer<pointcloud::Vertex<Scalar>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Mark,
            mesh::Name,
            mesh::TexturePaths,
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
