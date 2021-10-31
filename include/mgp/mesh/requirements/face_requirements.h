/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_REQUIREMENTS_H
#define MGP_MESH_FACE_REQUIREMENTS_H

namespace mgp {

// is/has functions

template<typename MeshType>
bool constexpr hasPerFaceVertexReferences();

template<typename MeshType>
bool constexpr hasPerFaceVertexReferences(const MeshType&);

template <typename MeshType>
bool hasPerFaceNormal(const MeshType&);

template <typename MeshType>
bool hasPerFaceColor(const MeshType&);

template <typename MeshType>
bool hasPerFaceScalar(const MeshType&);

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents();

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents(const MeshType&);

template <typename MeshType>
bool hasPerFaceMutableBitFlags(const MeshType&);

// require functions

template <typename MeshType>
void constexpr requirePerFaceVertexReferences();
template <typename MeshType>
void constexpr requirePerFaceVertexReferences(const MeshType&);

template <typename MeshType>
void requirePerFaceNormal(const MeshType& m);

template <typename MeshType>
void requirePerFaceColor(const MeshType& m);

template <typename MeshType>
void requirePerFaceScalar(const MeshType& m);

template <typename MeshType>
bool constexpr requirePerFaceCustomComponents();

template <typename MeshType>
bool constexpr requirePerFaceCustomComponents(const MeshType& m);

template <typename MeshType>
void requirePerFaceMutableBitFlags(const MeshType& m);

}

#include "face_requirements.cpp"

#endif // MGP_MESH_FACE_REQUIREMENTS_H
