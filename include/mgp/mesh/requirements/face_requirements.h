/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_REQUIREMENTS_H
#define MGP_MESH_FACE_REQUIREMENTS_H

namespace mgp {

// is/has functions

template<typename MeshType>
bool constexpr hasPerFaceVertexReferencesArray();

template<typename MeshType>
bool constexpr hasPerFaceVertexReferencesArray(const MeshType&);

// require functions

template <typename MeshType>
void constexpr requirePerFaceVertexReferencesArray();
template <typename MeshType>
void constexpr requirePerFaceVertexReferencesArray(const MeshType&);

}

#include "face_requirements.cpp"

#endif // MGP_MESH_FACE_REQUIREMENTS_H
