/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_requirements.h"

#include <mgp/exception/mesh_exception.h>

#include "element_requirements.h"
#include "../container/face_container.h"

namespace mgp {

template<typename MeshType>
bool constexpr hasPerFaceVertexReferencesArray()
{
	return hasFaces<MeshType>() && mgp::face::hasVertexRefsArray<typename MeshType::FaceType>();
}

template<typename MeshType>
bool constexpr hasPerFaceVertexReferencesArray(const MeshType&)
{
	return hasPerFaceVertexReferencesArray<MeshType>();
}

template<typename MeshType>
void constexpr requirePerFaceVertexReferencesArray()
{
	static_assert(
		hasPerFaceVertexReferencesArray<MeshType>,
		"Mesh Face component has no fixed-size Vertex references.");
}

template<typename MeshType>
void constexpr requirePerFaceVertexReferencesArray(const MeshType&)
{
	requirePerFaceVertexReferencesArray<MeshType>();
}

}
