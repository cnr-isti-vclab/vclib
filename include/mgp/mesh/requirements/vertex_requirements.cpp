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

template <typename MeshType>
bool enableIfPerVertexNormalOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalNormal<typename MeshType::VertexType>()) {
		m.enablePerVertexNormal();
		return true;
	}
	else {
		return hasPerVertexNormal<MeshType>();
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
bool enableIfPerVertexColorOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalColor<typename MeshType::VertexType>()) {
		m.enablePerVertexColor();
		return true;
	}
	else {
		return hasPerVertexColor<MeshType>();
	}
}

template <typename MeshType>
bool constexpr hasPerVertexScalar()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasScalar<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalScalar<typename MeshType::VertexType>());
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
bool enableIfPerVertexScalarOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
		m.enablePerVertexScalar();
		return true;
	}
	else {
		return hasPerVertexScalar<MeshType>();
	}
}

template <typename MeshType>
bool constexpr hasPerVertexTexCoord()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasTexCoord<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalTexCoord<typename MeshType::VertexType>());
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
bool enableIfPerVertexTexCoordOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalTexCoord<typename MeshType::VertexType>()) {
		m.enablePerVertexTexCoord();
		return true;
	}
	else {
		return hasPerVertexTexCoord<MeshType>();
	}
}

template <typename MeshType>
bool constexpr hasPerVertexAdjacentFaces()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasAdjacentFaces<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>());
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
bool enableIfPerVertexAdjacentFacesOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() &&
		mgp::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>()) {
		m.enablePerVertexAdjacentFaces();
		return true;
	}
	else {
		return hasPerVertexAdjacentFaces<MeshType>();
	}
}

template <typename MeshType>
bool constexpr hasPerVertexAdjacentVertices()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasAdjacentVertices<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>());
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

template <typename MeshType>
bool enableIfPerVertexAdjacentVerticesOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() &&
		mgp::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>()) {
		m.enablePerVertexAdjacentVertices();
		return true;
	}
	else {
		return hasPerVertexAdjacentVertices<MeshType>();
	}
}

template<typename MeshType>
bool constexpr hasPerVertexCustomComponents()
{
	return hasVertices<MeshType>() &&
		   mgp::vert::hasCustomComponents<typename MeshType::VertexType>();
}

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags()
{
	return hasVertices<MeshType>() &&
		   (mgp::vert::hasMutableBitFlags<typename MeshType::VertexType>() ||
			mgp::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>());
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

template <typename MeshType>
bool enableIfPerVertexMutableBitFlagsOptional(MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && mgp::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>()) {
		m.enablePerVertexMutableBitFlags();
		return true;
	}
	else {
		return hasPerVertexMutableBitFlags<MeshType>();
	}
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexNormal<MeshType>(), "Mesh has no vertex normals.");
	if (!isPerVertexNormalEnabled(m))
		throw mgp::MissingComponentException("Vertex normals not enabled.");
}

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexColor<MeshType>(), "Mesh has no vertex colors.");
	if (!isPerVertexColorEnabled(m))
		throw mgp::MissingComponentException("Vertex colors not enabled.");
}

template<typename MeshType>
void requirePerVertexScalar(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexScalar<MeshType>(), "Mesh has no vertex scalars.");
	if (!isPerVertexScalarEnabled(m))
		throw mgp::MissingComponentException("Vertex scalars not enabled.");
}

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexTexCoord<MeshType>(), "Mesh has no vertex texcoords.");
	if (!isPerVertexTexCoordEnabled(m))
		throw mgp::MissingComponentException("Vertex texcoords not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexAdjacentFaces<MeshType>(), "Mesh has no vertex adjacent faces.");
	if (!isPerVertexAdjacentFacesEnabled(m))
		throw mgp::MissingComponentException("Vertex adjacent faces not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexAdjacentVertices<MeshType>(), "Mesh has no per vertex adjacent vertices.");
	if (!isPerVertexAdjacentVerticesEnabled(m))
		throw mgp::MissingComponentException("Per vertex adjacent vertices not enabled.");
}

template<typename MeshType>
void constexpr requirePerVertexCustomComponents()
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexCustomComponents<MeshType>(), "Mesh has no vertex custom components.");
}

template<typename MeshType>
void requirePerVertexCustomComponents(const MeshType&)
{
	requirePerVertexCustomComponents<MeshType>();
}

template<typename MeshType>
void requirePerVertexMutableBitFlags(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexMutableBitFlags<MeshType>(), "Mesh has no per vertex mutable bit flags.");
	if (!isPerVertexMutableBitFlagsEnabled(m))
		throw mgp::MissingComponentException("Vertex mutable bit flags not enabled.");
}

} // namespace mgp
