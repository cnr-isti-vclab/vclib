/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "element_requirements.h"

#include "../mesh.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasVertices()
{
	return mgp::mesh::hasVertices<MeshType>();
}

template<typename MeshType>
bool hasVertices(const MeshType&)
{
	return hasVertices<MeshType>();
}

template<typename MeshType>
bool constexpr hasFaces()
{
	return mgp::mesh::hasFaces<MeshType>();
}

template<typename MeshType>
bool hasFaces(const MeshType&)
{
	return hasFaces<MeshType>();
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void constexpr requireVertices()
{
	static_assert(hasVertices<MeshType>(), "Mesh does not have a Vertex Container.");
}

template<typename MeshType>
void requireVertices(const MeshType&)
{
	requireVertices<MeshType>();
}

template<typename MeshType>
void constexpr requireFaces()
{
	static_assert(hasFaces<MeshType>(), "Mesh does not have a Face Container.");
}

template<typename MeshType>
void requireFaces(const MeshType&)
{
	requireFaces<MeshType>();
}

}
