/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_REQUIREMENTS_H
#define MGP_MESH_VERTEX_REQUIREMENTS_H

#include "element_requirements.h"

namespace mgp {

// is/has functions

template <typename MeshType>
bool constexpr hasPerVertexCoordinate();

template <typename MeshType>
bool constexpr hasPerVertexCoordinate(const MeshType&);

template <typename MeshType>
bool constexpr hasPerVertexNormal();

template <typename MeshType>
bool constexpr hasPerVertexNormal(const MeshType&);

template <typename MeshType>
bool isPerVertexNormalEnabled(const MeshType&);

template <typename MeshType>
bool constexpr hasPerVertexColor();

template <typename MeshType>
bool constexpr hasPerVertexColor(const MeshType&);

template <typename MeshType>
bool isPerVertexColorEnabled(const MeshType&);

template <typename MeshType>
bool constexpr hasPerVertexScalar();

template <typename MeshType>
bool constexpr hasPerVertexScalar(const MeshType&);

template <typename MeshType>
bool isPerVertexScalarEnabled(const MeshType&);

template <typename MeshType>
bool constexpr hasPerVertexCustomProperties();

template <typename MeshType>
bool constexpr hasPerVertexCustomProperties(const MeshType&);

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces();

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces(const MeshType&);

template <typename MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType&);

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags();

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags(const MeshType&);

template <typename MeshType>
bool isPerVertexMutableBitFlagsEnabled(const MeshType&);

// require functions

template <typename MeshType>
void constexpr requirePerVertexCoordinate();
template <typename MeshType>
void constexpr requirePerVertexCoordinate(const MeshType&);

template <typename MeshType>
void requirePerVertexNormal(const MeshType& m);

template <typename MeshType>
void requirePerVertexColor(const MeshType& m);

template <typename MeshType>
void requirePerVertexScalar(const MeshType& m);

template <typename MeshType>
bool constexpr requirePerVertexCustomProperties();

template <typename MeshType>
bool constexpr requirePerVertexCustomProperties(const MeshType& m);

template <typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m);

template <typename MeshType>
void requirePerVertexMutableBitFlags(const MeshType& m);

}

#include "vertex_requirements.cpp"

#endif // MGP_MESH_VERTEX_REQUIREMENTS_H
