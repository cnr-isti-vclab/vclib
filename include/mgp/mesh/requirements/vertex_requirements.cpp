/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_requirements.h"

#include <mgp/exception/mesh_exception.h>

#include "../container/vertex_container.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasPerVertexNormal()
{
	return hasVertices<MeshType>() && (
		mgp::vert::hasNormal<typename MeshType::VertexType>() ||
		mgp::vert::hasOptionalNormal<typename MeshType::VertexType>());
}

template<typename MeshType>
bool constexpr hasPerVertexNormal(const MeshType&)
{
	return hasPerVertexNormal<MeshType>();
}

template<typename MeshType>
bool isPerVertexNormalEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasNormal<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalNormal<typename MeshType::VertexType>()) {
		return m.isPerVertexNormalEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerVertexColor()
{
	return hasVertices<MeshType>() && (
			mgp::vert::hasColor<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalColor<typename MeshType::VertexType>());
}

template<typename MeshType>
bool constexpr hasPerVertexColor(const MeshType&)
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasColor<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalColor<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexColorEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasColor<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalColor<typename MeshType::VertexType>()) {
		return m.isPerVertexColorEnabled();
	}
	else {
		return false;
	}
}

template <typename MeshType>
bool constexpr hasPerVertexScalar()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasScalar<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalScalar<typename MeshType::VertexType>());
}

template <typename MeshType>
bool constexpr hasPerVertexScalar(const MeshType&)
{
	return hasPerVertexScalar<MeshType>();
}

template<typename MeshType>
bool isPerVertexScalarEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasScalar<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
		return m.isPerVertexScalarEnabled();
	}
	else {
		return false;
	}
}

template <typename MeshType>
bool constexpr hasPerVertexTexCoord()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasTexCoord<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalTexCoord<typename MeshType::VertexType>());
}

template <typename MeshType>
bool constexpr hasPerVertexTexCoord(const MeshType&)
{
	return hasPerVertexTexCoord<MeshType>();
}

template<typename MeshType>
bool isPerVertexTexCoordEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasTexCoord<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalTexCoord<typename MeshType::VertexType>()) {
		return m.isPerVertexTexCoordEnabled();
	}
	else {
		return false;
	}
}

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasAdjacentFaces<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>());
}

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces(const MeshType&)
{
	return hasPerVertexAdjacentFaces<MeshType>();
}

template<typename MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasAdjacentFaces<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() &&
		mgp::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>()) {
		return m.isPerVertexAdjacentFacesEnabled();
	}
	else {
		return false;
	}
}

template <typename MeshType>
bool constexpr hasPerVertexAdjacentVertices()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasAdjacentVertices<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>());
}

template <typename MeshType>
bool constexpr hasPerVertexAdjacentVertices(const MeshType&)
{
	return hasPerVertexAdjacentVertices<MeshType>();
}

template<typename MeshType>
bool isPerVertexAdjacentVerticesEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasAdjacentVertices<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() &&
		mgp::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>()) {
		return m.isPerVertexAdjacentVerticesEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerVertexCustomProperties()
{
	return hasVertices<MeshType>() &&
		   mgp::vert::hasCustomProperties<typename MeshType::VertexType>();
}

template<typename MeshType>
bool constexpr hasPerVertexCustomProperties(const MeshType&)
{
	return hasPerVertexCustomProperties<MeshType>();
}

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasMutableBitFlags<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>());
}

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags(const MeshType&)
{
	return hasPerVertexMutableBitFlags<MeshType>();
}

template<typename MeshType>
bool isPerVertexMutableBitFlagsEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasMutableBitFlags<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>()) {
		return m.isPerVertexMutableBitFlagsEnabled();
	}
	else {
		return false;
	}
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexNormal(m), "Mesh has no vertex normals.");
	if (!isPerVertexNormalEnabled(m))
		throw mgp::MissingPropertyException("Vertex normals not enabled.");
}

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexColor(m), "Mesh has no vertex colors.");
	if (!isPerVertexColorEnabled(m))
		throw mgp::MissingPropertyException("Vertex colors not enabled.");
}

template<typename MeshType>
void requirePerVertexScalar(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexScalar(m), "Mesh has no vertex scalars.");
	if (!isPerVertexScalarEnabled(m))
		throw mgp::MissingPropertyException("Vertex scalars not enabled.");
}

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexTexCoord(m), "Mesh has no vertex texcoords.");
	if (!isPerVertexTexCoordEnabled(m))
		throw mgp::MissingPropertyException("Vertex texcoords not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexAdjacentFaces(m), "Mesh has no vertex adjacent faces.");
	if (!isPerVertexAdjacentFacesEnabled(m))
		throw mgp::MissingPropertyException("Vertex adjacent faces not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexAdjacentVertices(m), "Mesh has no per vertex adjacent vertices.");
	if (!isPerVertexAdjacentVerticesEnabled(m))
		throw mgp::MissingPropertyException("Per vertex adjacent vertices not enabled.");
}

template<typename MeshType>
void constexpr requirePerVertexCustomProperties()
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexCustomProperties<MeshType>(), "Mesh has no vertex custom properties.");
}

template<typename MeshType>
void constexpr requirePerVertexCustomProperties(const MeshType&)
{
	requirePerVertexCustomProperties<MeshType>();
}

template<typename MeshType>
void requirePerVertexMutableBitFlags(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexMutableBitFlags(m), "Mesh has no per vertex mutable bit flags.");
	if (!isPerVertexMutableBitFlagsEnabled(m))
		throw mgp::MissingPropertyException("Vertex mutable bit flags not enabled.");
}

} // namespace mgp
