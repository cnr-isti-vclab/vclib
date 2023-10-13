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

#ifndef VCL_MESHES_POLY_MESH_H
#define VCL_MESHES_POLY_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename ScalarType>
class PolyMeshT;

} // namespace vcl

namespace vcl::polymesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Face;

template<typename Scalar>
class Vertex :
        public vcl::Vertex<
            PolyMeshT<Scalar>,
            vcl::vert::BitFlags,
            vcl::vert::Coordinate3<Scalar>,
            vcl::vert::Normal3<Scalar>,
            vcl::vert::Color,
            vcl::vert::Quality<Scalar>,
            vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentFaces<Face<Scalar>, Vertex<Scalar>>,
            vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,
            vcl::vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
            vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
        public vcl::Face<
            PolyMeshT<Scalar>,
            vcl::face::PolygonBitFlags, // 4b
            vcl::face::PolygonVertexPtrs<Vertex<Scalar>>,
            vcl::face::Normal3<Scalar>,
            vcl::face::OptionalColor<Face<Scalar>>,
            vcl::face::OptionalQuality<Scalar, Face<Scalar>>,
            vcl::face::OptionalAdjacentPolygons<Face<Scalar>>,
            vcl::face::OptionalPolygonWedgeTexCoords<Scalar, Face<Scalar>>,
            vcl::face::CustomComponents<Face<Scalar>>>
{
};

} // namespace vcl::polymesh

namespace vcl {

template<typename Scalar = double>
class PolyMeshT :
        public vcl::Mesh<
            mesh::VertexContainer<polymesh::Vertex<Scalar>>,
            mesh::FaceContainer<polymesh::Face<Scalar>>,
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

using PolyMeshf = PolyMeshT<float>;
using PolyMesh  = PolyMeshT<double>;

} // namespace vcl

#endif // VCL_MESHES_POLY_MESH_H
