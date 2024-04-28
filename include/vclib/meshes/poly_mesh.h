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
            vert::BitFlags,
            vert::Coordinate3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar>>,
            vert::OptionalAdjacentFaces<Face<Scalar>, Vertex<Scalar>>,
            vert::OptionalAdjacentVertices<Vertex<Scalar>>,
            vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
            vert::OptionalMark<Vertex<Scalar>>,
            vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
        public vcl::Face<
            PolyMeshT<Scalar>,
            face::PolygonBitFlags, // 4b
            face::PolygonVertexPtrs<Vertex<Scalar>>,
            face::Normal3<Scalar>,
            face::OptionalColor<Face<Scalar>>,
            face::OptionalQuality<Scalar, Face<Scalar>>,
            face::OptionalAdjacentPolygons<Face<Scalar>>,
            face::OptionalPolygonWedgeTexCoords<Scalar, Face<Scalar>>,
            face::OptionalMark<Face<Scalar>>,
            face::CustomComponents<Face<Scalar>>>
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
