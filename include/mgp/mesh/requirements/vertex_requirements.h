/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_REQUIREMENTS_H
#define MGP_MESH_VERTEX_REQUIREMENTS_H

namespace mgp {

// is/has functions

template <typename MeshType>
bool constexpr hasPerVertexCoordinate();

template <typename MeshType>
bool constexpr hasPerVertexCoordinate(const MeshType&);

template <typename MeshType>
bool hasPerVertexNormal(const MeshType&);

template <typename MeshType>
bool hasPerVertexScalar(const MeshType&);

template <typename MeshType>
bool constexpr hasPerVertexCustomComponents();

template <typename MeshType>
bool constexpr hasPerVertexCustomComponents(const MeshType&);

template <typename MeshType>
bool hasPerVertexFaceAdjacencies(const MeshType&);

// require functions

template <typename MeshType>
void constexpr requirePerVertexCoordinate();
template <typename MeshType>
void constexpr requirePerVertexCoordinate(const MeshType&);

template <typename MeshType>
void requirePerVertexNormal(const MeshType& m);

}

#include "vertex_requirements.cpp"

#endif // MGP_MESH_VERTEX_REQUIREMENTS_H
