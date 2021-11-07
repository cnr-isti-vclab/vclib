/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_REQUIREMENTS_H
#define MGP_MESH_VERTEX_REQUIREMENTS_H

#include "element_requirements.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

// Vertex Normal

template <typename MeshType>
bool constexpr hasPerVertexNormal();

template <typename MeshType>
bool constexpr hasPerVertexNormal(const MeshType&);

template <typename MeshType>
bool isPerVertexNormalEnabled(const MeshType&);

// Vertex Color

template <typename MeshType>
bool constexpr hasPerVertexColor();

template <typename MeshType>
bool constexpr hasPerVertexColor(const MeshType&);

template <typename MeshType>
bool isPerVertexColorEnabled(const MeshType&);

// Vertex Scalar

template <typename MeshType>
bool constexpr hasPerVertexScalar();

template <typename MeshType>
bool constexpr hasPerVertexScalar(const MeshType&);

template <typename MeshType>
bool isPerVertexScalarEnabled(const MeshType&);

// Vertex TexCoord

template <typename MeshType>
bool constexpr hasPerVertexTexCoord();

template <typename MeshType>
bool constexpr hasPerVertexTexCoord(const MeshType&);

template <typename MeshType>
bool isPerVertexTexCoordEnabled(const MeshType&);

// Vertex Adjacent Faces

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces();

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces(const MeshType&);

template <typename MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType&);

// Vertex Adjacent Vertices

template <typename MeshType>
bool constexpr hasPerVertexAdjacentVertices();

template <typename MeshType>
bool constexpr hasPerVertexAdjacentVertices(const MeshType&);

template <typename MeshType>
bool isPerVertexAdjacentVerticesEnabled(const MeshType&);

// Vertex Custom Properties

template <typename MeshType>
bool constexpr hasPerVertexCustomProperties();

template <typename MeshType>
bool constexpr hasPerVertexCustomProperties(const MeshType&);

// Vertex Mutable Flags

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags();

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags(const MeshType&);

template <typename MeshType>
bool isPerVertexMutableBitFlagsEnabled(const MeshType&);


/*********************
 * require functions *
 *********************/

// Vertex Normal

template <typename MeshType>
void requirePerVertexNormal(const MeshType& m);

// Vertex Color

template <typename MeshType>
void requirePerVertexColor(const MeshType& m);

// Vertex Scalar

template <typename MeshType>
void requirePerVertexScalar(const MeshType& m);

// Vertex TexCoord

template <typename MeshType>
void requirePerVertexTexCoord(const MeshType& m);

// Vertex Adjacent Faces

template <typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m);

// Vertex Adjacent Vertices

template <typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m);

// Vertex Custom Properties

template <typename MeshType>
bool constexpr requirePerVertexCustomProperties();

template <typename MeshType>
bool constexpr requirePerVertexCustomProperties(const MeshType& m);

// Vertex Mutable Flags

template <typename MeshType>
void requirePerVertexMutableBitFlags(const MeshType& m);

}

#include "vertex_requirements.cpp"

#endif // MGP_MESH_VERTEX_REQUIREMENTS_H
