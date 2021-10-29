/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_REQUIREMENTS_H
#define MGP_MESH_REQUIREMENTS_H

namespace mgp {

template<typename MeshType>
void constexpr requireMeshVertices();
template<typename MeshType>
void constexpr requireMeshVertices(const MeshType&);

template<typename MeshType>
void constexpr requireMeshFaces();
template<typename MeshType>
void constexpr requireMeshFaces(const MeshType&);

template <typename MeshType>
void constexpr requireTriangleMesh();
template <typename MeshType>
void constexpr requireTriangleMesh(const MeshType&);

template <typename MeshType>
void constexpr requireQuadMesh();
template <typename MeshType>
void constexpr requireQuadMesh(const MeshType&);

template <typename MeshType>
void constexpr requirePerVertexCoordinates();
template <typename MeshType>
void constexpr requirePerVertexCoordinates(const MeshType&);

template <typename MeshType>
void requirePerVertexNormals(const MeshType& m);

template <typename MeshType>
void constexpr requirePerFaceVertexReferences();
template <typename MeshType>
void constexpr requirePerFaceVertexReferences(const MeshType&);

}

#include "requirements.cpp"

#endif // MGP_MESH_REQUIREMENTS_H
