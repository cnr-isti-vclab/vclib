/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "vertex_requirements.h"

#include <vclib/exception/mesh_exception.h>

#include "../container/vertex_container.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasPerVertexNormal()
{
	return hasVertices<MeshType>() && (
		vcl::vert::hasNormal<typename MeshType::VertexType>() ||
		vcl::vert::hasOptionalNormal<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexNormalEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasNormal<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasOptionalNormal<typename MeshType::VertexType>()) {
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
		hasVertices<MeshType>() && vcl::vert::hasOptionalNormal<typename MeshType::VertexType>()) {
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
			vcl::vert::hasColor<typename MeshType::VertexType>() ||
			vcl::vert::hasOptionalColor<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexColorEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasColor<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasOptionalColor<typename MeshType::VertexType>()) {
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
		hasVertices<MeshType>() && vcl::vert::hasOptionalColor<typename MeshType::VertexType>()) {
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
		   (vcl::vert::hasScalar<typename MeshType::VertexType>() ||
			vcl::vert::hasOptionalScalar<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexScalarEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasScalar<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
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
		hasVertices<MeshType>() && vcl::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
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
		   (vcl::vert::hasTexCoord<typename MeshType::VertexType>() ||
			vcl::vert::hasOptionalTexCoord<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexTexCoordEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasTexCoord<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasOptionalTexCoord<typename MeshType::VertexType>()) {
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
		hasVertices<MeshType>() && vcl::vert::hasOptionalTexCoord<typename MeshType::VertexType>()) {
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
		   (vcl::vert::hasAdjacentFaces<typename MeshType::VertexType>() ||
			vcl::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasAdjacentFaces<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() &&
		vcl::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>()) {
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
		vcl::vert::hasOptionalAdjacentFaces<typename MeshType::VertexType>()) {
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
		   (vcl::vert::hasAdjacentVertices<typename MeshType::VertexType>() ||
			vcl::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexAdjacentVerticesEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasAdjacentVertices<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() &&
		vcl::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>()) {
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
		vcl::vert::hasOptionalAdjacentVertices<typename MeshType::VertexType>()) {
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
		   vcl::vert::hasCustomComponents<typename MeshType::VertexType>();
}

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags()
{
	return hasVertices<MeshType>() &&
		   (vcl::vert::hasMutableBitFlags<typename MeshType::VertexType>() ||
			vcl::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>());
}

template<typename MeshType>
bool isPerVertexMutableBitFlagsEnabled(const MeshType& m)
{
	if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasMutableBitFlags<typename MeshType::VertexType>()) {
		return true;
	}
	else if constexpr (
		hasVertices<MeshType>() && vcl::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>()) {
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
		hasVertices<MeshType>() && vcl::vert::hasOptionalMutableBitFlags<typename MeshType::VertexType>()) {
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
		throw vcl::MissingComponentException("Vertex normals not enabled.");
}

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexColor<MeshType>(), "Mesh has no vertex colors.");
	if (!isPerVertexColorEnabled(m))
		throw vcl::MissingComponentException("Vertex colors not enabled.");
}

template<typename MeshType>
void requirePerVertexScalar(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexScalar<MeshType>(), "Mesh has no vertex scalars.");
	if (!isPerVertexScalarEnabled(m))
		throw vcl::MissingComponentException("Vertex scalars not enabled.");
}

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexTexCoord<MeshType>(), "Mesh has no vertex texcoords.");
	if (!isPerVertexTexCoordEnabled(m))
		throw vcl::MissingComponentException("Vertex texcoords not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexAdjacentFaces<MeshType>(), "Mesh has no vertex adjacent faces.");
	if (!isPerVertexAdjacentFacesEnabled(m))
		throw vcl::MissingComponentException("Vertex adjacent faces not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexAdjacentVertices<MeshType>(), "Mesh has no per vertex adjacent vertices.");
	if (!isPerVertexAdjacentVerticesEnabled(m))
		throw vcl::MissingComponentException("Per vertex adjacent vertices not enabled.");
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
		throw vcl::MissingComponentException("Vertex mutable bit flags not enabled.");
}

} // namespace vcl
