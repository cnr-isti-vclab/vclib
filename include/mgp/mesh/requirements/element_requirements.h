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

template<typename MeshType>
bool constexpr hasVertices(const MeshType&);

// Mesh Faces

template<typename MeshType>
bool constexpr hasFaces();

template<typename MeshType>
bool constexpr hasFaces(const MeshType&);

/*********************
 * require functions *
 *********************/

// Mesh Vertices

template<typename MeshType>
void constexpr requireVertices();
template<typename MeshType>
void constexpr requireVertices(const MeshType&);

// Mesh Faces

template<typename MeshType>
void constexpr requireFaces();
template<typename MeshType>
void constexpr requireFaces(const MeshType&);

}

#include "element_requirements.cpp"

#endif // MGP_MESH_ELEMENT_REQUIREMENTS_H
