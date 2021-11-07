/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_REQUIREMENTS_H
#define MGP_MESH_FACE_REQUIREMENTS_H

#include "element_requirements.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

// Face Normal

template <typename MeshType>
bool constexpr hasPerFaceNormal();

template <typename MeshType>
bool constexpr hasPerFaceNormal(const MeshType&);

template <typename MeshType>
bool isPerFaceNormalEnabled(const MeshType&);

// Face Color

template <typename MeshType>
bool constexpr hasPerFaceColor();

template <typename MeshType>
bool constexpr hasPerFaceColor(const MeshType&);

template <typename MeshType>
bool isPerFaceColorEnabled(const MeshType&);

// Face Scalar

template <typename MeshType>
bool constexpr hasPerFaceScalar();

template <typename MeshType>
bool constexpr hasPerFaceScalar(const MeshType&);

template <typename MeshType>
bool isPerFaceScalarEnabled(const MeshType&);

// Face Adjacent Faces

template <typename MeshType>
bool constexpr hasPerFaceAdjacentFaces();

template <typename MeshType>
bool constexpr hasPerFaceAdjacentFaces(const MeshType&);

template <typename MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType&);

// Face Custom Properties

template<typename MeshType>
bool constexpr hasPerFaceCustomProperties();

template<typename MeshType>
bool constexpr hasPerFaceCustomProperties(const MeshType&);

// Face Mutable Flags

template <typename MeshType>
bool constexpr hasPerFaceMutableBitFlags();

template <typename MeshType>
bool constexpr hasPerFaceMutableBitFlags(const MeshType&);

template <typename MeshType>
bool isPerFaceMutableBitFlagsEnabled(const MeshType&);

/*********************
 * require functions *
 *********************/

// Face Normal

template <typename MeshType>
void requirePerFaceNormal(const MeshType& m);

// Face Color

template <typename MeshType>
void requirePerFaceColor(const MeshType& m);

// Face Scalar

template <typename MeshType>
void requirePerFaceScalar(const MeshType& m);

// Face Adjacent Faces

template <typename MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m);

// Face Custom Properties

template <typename MeshType>
bool constexpr requirePerFaceCustomProperties();

template <typename MeshType>
bool constexpr requirePerFaceCustomProperties(const MeshType& m);

// Face Mutable Flags

template <typename MeshType>
void requirePerFaceMutableBitFlags(const MeshType& m);

}

#include "face_requirements.cpp"

#endif // MGP_MESH_FACE_REQUIREMENTS_H
