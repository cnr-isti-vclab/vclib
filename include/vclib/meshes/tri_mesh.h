/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_MESHES_TRI_MESH_H
#define VCL_MESHES_TRI_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename ScalarType>
class TriMeshT;

} // namespace vcl

namespace vcl::trimesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Face;

template<typename Scalar>
class Vertex :
        public vcl::Vertex<
            TriMeshT<Scalar>,
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vert::OptionalAdjacentFacePointers<Face<Scalar>, Vertex<Scalar>>,
            vert::OptionalAdjacentVertexPointers<Vertex<Scalar>>,
            vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vert::OptionalMark<Vertex<Scalar>>,
            vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
        public vcl::Face<
            TriMeshT<Scalar>,
            face::TriangleBitFlags,
            face::TriangleVertexPtrs<Vertex<Scalar>, Face<Scalar>>,
            face::Normal3<Scalar>,
            face::OptionalColor<Face<Scalar>>,
            face::OptionalQuality<Scalar, Face<Scalar>>,
            face::OptionalAdjacentTrianglePointers<Face<Scalar>>,
            face::OptionalTriangleWedgeTexCoords<Scalar, Face<Scalar>>,
            face::OptionalMark<Face<Scalar>>,
            face::CustomComponents<Face<Scalar>>>
{
};

} // namespace vcl::trimesh

namespace vcl {

/**
 * @brief The TriMeshT class
 * @ingroup meshes
 */
template<typename Scalar = double>
class TriMeshT :
        public vcl::Mesh<
            mesh::VertexContainer<trimesh::Vertex<Scalar>>,
            mesh::FaceContainer<trimesh::Face<Scalar>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Color,
            mesh::Mark,
            mesh::Name,
            mesh::TexturePaths,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    using ScalarType = Scalar;
};

/**
 * @brief The TriMeshf class is a specialization of TriMeshT that uses `float`
 * as scalar.
 * @ingroup meshes
 */
using TriMeshf = TriMeshT<float>;

/**
 * @brief The TriMesh class is a specialization of TriMeshT that uses `double`
 * as scalar.
 * @ingroup meshes
 */
using TriMesh = TriMeshT<double>;

} // namespace vcl

#endif // VCL_MESHES_TRI_MESH_H
