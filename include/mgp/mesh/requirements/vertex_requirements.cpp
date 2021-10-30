/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_requirements.h"

#include <mgp/exception/mesh_exception.h>

#include "../container/vertex_container.h"
#include "element_requirements.h"

namespace mgp {

template<typename MeshType>
bool constexpr hasPerVertexCoordinate()
{
	return hasVertices<MeshType>() && mgp::vert::hasCoordinate<typename MeshType::VertexType>();
}

template<typename MeshType>
bool constexpr hasPerVertexCoordinate(const MeshType&)
{
	return hasPerVertexCoordinate<MeshType>();
}

template<typename MeshType>
bool hasPerVertexNormal(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasNormal<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalNormal<typename MeshType::VertexType>()) {
		return m.isVertexNormalEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool hasPerVertexScalar(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasScalar<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
		return m.isVertexScalarEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerVertexCustomComponents()
{
	return hasVertices<MeshType>() &&
		   mgp::vert::hasCustomComponents<typename MeshType::VertexType>();
}

template<typename MeshType>
bool constexpr hasPerVertexCustomComponents(const MeshType&)
{
	return hasPerVertexCustomComponents<MeshType>();
}

template<typename MeshType>
bool hasPerVertexAdjacentFaces(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasAdjacentFaces<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>()) {
		return m.isVertexFaceAdjacenciesEnabled();
	}
	else {
		return false;
	}
}

// require functions

template<typename MeshType>
void constexpr requirePerVertexCoordinate()
{
	static_assert(hasPerVertexCoordinate<MeshType>(), "Mesh has no vertex coordinates.");
}

template<typename MeshType>
void constexpr requirePerVertexCoordinate(const MeshType&)
{
	requirePerVertexCoordinate<MeshType>();
}

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
{
	static_assert(
		mgp::vert::hasNormal<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalNormal<typename MeshType::VertexType>(),
		"Mesh has no vertex normals.");
	if (!hasPerVertexNormal(m))
		throw mgp::MissingComponentException("Vertex normals not enabled.");
}

} // namespace mgp
