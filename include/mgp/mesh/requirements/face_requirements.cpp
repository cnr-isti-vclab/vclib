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
	return hasFaces<MeshType>() && mgp::face::hasVertexReferences<typename MeshType::FaceType>();
}

template<typename MeshType>
bool constexpr hasPerFaceVertexReferencesArray(const MeshType&)
{
	return hasPerFaceVertexReferencesArray<MeshType>();
}

template<typename MeshType>
bool hasPerFaceNormal(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasNormal<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalNormal<typename MeshType::FaceType>()) {
		return m.isPerFaceNormalEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool hasPerFaceColor(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasColor<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalColor<typename MeshType::FaceType>()) {
		return m.isPerFaceColorEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool hasPerFaceScalar(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasScalar<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalScalar<typename MeshType::FaceType>()) {
		return m.isPerFaceScalarEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents()
{
	return hasFaces<MeshType>() && mgp::face::hasCustomComponents<typename MeshType::FaceType>();
}

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents(const MeshType&)
{
	return hasPerFaceCustomComponents<MeshType>();
}

template<typename MeshType>
bool hasPerFaceMutableBitFlags(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasMutableBitFlags<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>()) {
		return m.isPerFaceMutableBitFlagsEnabled();
	}
	else {
		return false;
	}
}

// require functions

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

template<typename MeshType>
void requirePerFaceNormal(const MeshType& m)
{
	static_assert(
		mgp::face::hasNormal<typename MeshType::FaceType>() ||
			mgp::face::hasOptionalNormal<typename MeshType::FaceType>(),
		"Mesh has no face normals.");
	if (!hasPerFaceNormal(m))
		throw mgp::MissingComponentException("Face normals not enabled.");
}

template<typename MeshType>
void requirePerFaceColor(const MeshType& m)
{
	static_assert(
		mgp::face::hasColor<typename MeshType::FaceType>() ||
			mgp::face::hasOptionalColor<typename MeshType::FaceType>(),
		"Mesh has no face colors.");
	if (!hasPerFaceColor(m))
		throw mgp::MissingComponentException("Face colors not enabled.");
}

template<typename MeshType>
void requirePerFaceScalar(const MeshType& m)
{
	static_assert(
		mgp::face::hasScalar<typename MeshType::FaceType>() ||
			mgp::face::hasOptionalScalar<typename MeshType::FaceType>(),
		"Mesh has no face scalars.");
	if (!hasPerFaceScalar(m))
		throw mgp::MissingComponentException("Face scalars not enabled.");
}

template<typename MeshType>
void constexpr requirePerFaceCustomComponents()
{
	static_assert(hasPerFaceCustomComponents<MeshType>(), "Mesh has no face custom components.");
}

template<typename MeshType>
void constexpr requirePerFaceCustomComponents(const MeshType&)
{
	requirePerFaceCustomComponents<MeshType>();
}

template<typename MeshType>
void requirePerFaceMutableBitFlags(const MeshType& m)
{
	static_assert(
		mgp::face::hasMutableBitFlags<typename MeshType::FaceType>() ||
			mgp::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>(),
		"Mesh has no per face mutable bit flags.");
	if (!hasPerFaceMutableBitFlags(m))
		throw mgp::MissingComponentException("Face mutable bit flags not enabled.");
}

}
