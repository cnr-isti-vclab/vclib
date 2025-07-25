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

#ifndef VCL_MESHES_POLY_MESH_H
#define VCL_MESHES_POLY_MESH_H

#include <vclib/mesh.h>

namespace vcl {

template<typename ScalarType, bool INDEXED>
class PolyMeshT;

} // namespace vcl

namespace vcl::polymesh {

template<typename Scalar, bool INDEXED>
class Vertex;

template<typename Scalar, bool INDEXED>
class Face;

/**
 * @brief The Vertex type used by the PolyMeshT class.
 *
 * @extends vert::BitFlags
 * @extends vert::Position3
 * @extends vert::Normal3
 * @extends vert::OptionalColor
 * @extends vert::OptionalQuality
 * @extends vert::OptionalAdjacentFaces
 * @extends vert::OptionalAdjacentVertices
 * @extends vert::OptionalPrincipalCurvature
 * @extends vert::OptionalTexCoord
 * @extends vert::OptionalMark
 * @extends vert::CustomComponents
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam I: A boolean flag that indicates whether the mesh uses indices or
 * pointers to store vertices of faces and adjacency information.
 *
 * @ingroup meshes
 */
template<typename Scalar, bool I>
class Vertex :
        public vcl::Vertex<
            PolyMeshT<Scalar, I>,
            vert::BitFlags,
            vert::Position3<Scalar>,
            vert::Normal3<Scalar>,
            vert::OptionalColor<Vertex<Scalar, I>>,
            vert::OptionalQuality<Scalar, Vertex<Scalar, I>>,
            vert::OptionalAdjacentFaces<I, Face<Scalar, I>, Vertex<Scalar, I>>,
            vert::OptionalAdjacentVertices<I, Vertex<Scalar, I>>,
            vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar, I>>,
            vert::OptionalTexCoord<Scalar, Vertex<Scalar, I>>,
            vert::OptionalMark<Vertex<Scalar, I>>,
            vert::CustomComponents<Vertex<Scalar, I>>>
{
};

/**
 * @brief The Face type used by the PolyMeshT class.
 *
 * @extends face::PolygonBitFlags
 * @extends face::PolygonVertexRefs
 * @extends face::Normal3
 * @extends face::OptionalColor
 * @extends face::OptionalQuality
 * @extends face::OptionalAdjacentPolygons
 * @extends face::OptionalPolygonWedgeTexCoords
 * @extends face::OptionalMark
 * @extends face::CustomComponents
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam I: A boolean flag that indicates whether the mesh uses indices or
 * pointers to store vertices of faces and adjacency information.
 *
 * @ingroup meshes
 */
template<typename Scalar, bool I>
class Face :
        public vcl::Face<
            PolyMeshT<Scalar, I>,
            face::PolygonBitFlags, // 4b
            face::PolygonVertexRefs<I, Vertex<Scalar, I>, Face<Scalar, I>>,
            face::Normal3<Scalar>,
            face::OptionalColor<Face<Scalar, I>>,
            face::OptionalQuality<Scalar, Face<Scalar, I>>,
            face::OptionalAdjacentPolygons<I, Face<Scalar, I>>,
            face::OptionalPolygonWedgeTexCoords<Scalar, Face<Scalar, I>>,
            face::OptionalMark<Face<Scalar, I>>,
            face::CustomComponents<Face<Scalar, I>>>
{
};

} // namespace vcl::polymesh

namespace vcl {

/**
 * @brief The PolyMeshT class is a mesh class that represents a polygonal mesh.
 *
 * It allows to store polymesh::Vertex and polymesh::Face elements.
 *
 * @tparam Scalar: The scalar type used for the mesh.
 * @tparam INDEXED: A boolean flag that indicates whether the mesh uses indices
 * or pointers to store references.
 *
 * @extends mesh::VertexContainer
 * @extends mesh::FaceContainer
 * @extends mesh::BoundingBox3
 * @extends mesh::Color
 * @extends mesh::Mark
 * @extends mesh::Name
 * @extends mesh::TextureImages
 * @extends mesh::TransformMatrix
 * @extends mesh::CustomComponents
 *
 * @ingroup meshes
 */
template<typename Scalar, bool INDEXED>
class PolyMeshT :
        public Mesh<
            mesh::VertexContainer<polymesh::Vertex<Scalar, INDEXED>>,
            mesh::FaceContainer<polymesh::Face<Scalar, INDEXED>>,
            mesh::BoundingBox3<Scalar>,
            mesh::Color,
            mesh::Mark,
            mesh::Name,
            mesh::TextureImages,
            mesh::TransformMatrix<Scalar>,
            mesh::CustomComponents>
{
public:
    /** @brief The scalar used to store all the data of the Mesh. */
    using ScalarType = Scalar;
};

/**
 * @brief The PolyMeshf class is a specialization of the PolyMeshT class that
 * uses `float` as scalar and pointers to store vertices of faces and adjacency
 * information.
 * @ingroup meshes
 */
using PolyMeshf = PolyMeshT<float, false>;

/**
 * @brief The PolyMesh class is a specialization of the PolyMeshT class that
 * uses `double` as scalar and pointers to store vertices of faces and adjacency
 * information.
 * @ingroup meshes
 */
using PolyMesh = PolyMeshT<double, false>;

/**
 * @brief The PolyMeshIndexedf class is a specialization of the PolyMeshT class
 * that uses `float` as scalar and indices (`unsigned int`) to store vertices of
 * faces and adjacency information.
 * @ingroup meshes
 */
using PolyMeshIndexedf = PolyMeshT<float, true>;

/**
 * @brief The PolyMeshIndexed class is a specialization of the PolyMeshT class
 * that uses `double` as scalar and indices (`unsigned int`) to store vertices
 * of faces and adjacency information.
 * @ingroup meshes
 */
using PolyMeshIndexed = PolyMeshT<double, true>;

} // namespace vcl

#endif // VCL_MESHES_POLY_MESH_H
