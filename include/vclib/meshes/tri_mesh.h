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
            vcl::vert::BitFlags,
            vcl::vert::Coordinate3<Scalar>,
            vcl::vert::Normal3<Scalar>,
            vcl::vert::Color,
            vcl::vert::Quality<Scalar>,
            vcl::vert::OptionalAdjacentFaces<Face<Scalar>, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,
            vcl::vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalMark<Vertex<Scalar>>,
            vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
        public vcl::Face<
            TriMeshT<Scalar>,
            vcl::face::TriangleBitFlags,
            vcl::face::TriangleVertexPtrs<Vertex<Scalar>>,
            vcl::face::Normal3<Scalar>,
            vcl::face::OptionalQuality<Scalar, Face<Scalar>>,
            vcl::face::OptionalColor<Face<Scalar>>,
            vcl::face::OptionalAdjacentTriangles<Face<Scalar>>,
            vcl::face::OptionalTriangleWedgeTexCoords<Scalar, Face<Scalar>>,
            vcl::face::OptionalMark<Face<Scalar>>,
            vcl::face::CustomComponents<Face<Scalar>>>
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
