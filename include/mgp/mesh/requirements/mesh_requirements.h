/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_REQUIREMENTS_H
#define MGP_MESH_MESH_REQUIREMENTS_H

#include "face_requirements.h"
#include "vertex_requirements.h"

namespace mgp {

// is/has functions

template <typename MeshType>
bool constexpr isTriangleMesh();

template <typename MeshType>
bool constexpr isTriangleMesh(const MeshType&);

template <typename MeshType>
bool constexpr isQuadMesh();

template <typename MeshType>
bool constexpr isQuadMesh(const MeshType&);

// require functions

template <typename MeshType>
void constexpr requireTriangleMesh();
template <typename MeshType>
void constexpr requireTriangleMesh(const MeshType&);

template <typename MeshType>
void constexpr requireQuadMesh();
template <typename MeshType>
void constexpr requireQuadMesh(const MeshType&);

}

#include "mesh_requirements.cpp"

#endif // MGP_MESH_MESH_REQUIREMENTS_H
