/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENT_REQUIREMENTS_H
#define MGP_MESH_ELEMENT_REQUIREMENTS_H

namespace mgp {

/********************
 * is/has functions *
 ********************/

// Mesh Vertices

template<typename MeshType>
bool constexpr hasVertices();

// Mesh Faces

template<typename MeshType>
bool constexpr hasFaces();

/*********************
 * require functions *
 *********************/

// Mesh Vertices

template<typename MeshType>
void constexpr requireVertices();

// Mesh Faces

template<typename MeshType>
void constexpr requireFaces();

}

#include "element_requirements.cpp"

#endif // MGP_MESH_ELEMENT_REQUIREMENTS_H
