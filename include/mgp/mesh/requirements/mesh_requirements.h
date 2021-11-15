/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_REQUIREMENTS_H
#define MGP_MESH_MESH_REQUIREMENTS_H

#include "face_requirements.h"
#include "vertex_requirements.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

// Triangles

template <typename MeshType>
bool constexpr hasTriangles();

template <typename MeshType>
bool isTriangleMesh(const MeshType&);

// Quads

template <typename MeshType>
bool constexpr hasQuads();

template <typename MeshType>
bool isQuadMesh(const MeshType&);

// Bounding Box

template <typename MeshType>
bool constexpr hasBoundingBox();

/*********************
 * require functions *
 *********************/

// Triangles

template <typename MeshType>
void requireTriangleMesh(const MeshType&);

// Quads

template <typename MeshType>
void requireQuadMesh(const MeshType&);

// Bounding Box

template <typename MeshType>
void constexpr requireBoundingBox();

}

#include "mesh_requirements.cpp"

#endif // MGP_MESH_MESH_REQUIREMENTS_H
