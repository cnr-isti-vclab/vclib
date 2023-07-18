/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MESH_CONTAINER_FACE_CONTAINER_H
#define VCL_MESH_CONTAINER_FACE_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/face.h>
#include <vclib/mesh/elements/face_components.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The FaceContainer class represents a container of Face elements that
 * can be used in a Mesh class.
 *
 * This class adds a container (vector) of Faces to the Mesh, making available
 * the accessors members to them and their components.
 *
 * It provides all the add, delete, reserve, compact and other functions to
 * manage the Faces, plus the iterators and views to iterate over them. It also
 * provides the functions to enable and disable the optional components of the
 * Faces.
 *
 * @tparam T: The type of the Face elements. It must satisfy the FaceConcept.
 *
 * @ingroup containers
 */
template<FaceConcept T>
class FaceContainer : public ElementContainer<T>
{
	template <FaceConcept U>
	friend class FaceContainer;

	using FaceContainerType = FaceContainer<T>;
	using Base = ElementContainer<T>;

public:
	using Face              = T;
	using FaceType          = T;
	using FaceIterator      = typename Base::ElementIterator;
	using ConstFaceIterator = typename Base::ConstElementIterator;

	FaceContainer();

	const FaceType& face(uint i) const;
	FaceType&       face(uint i);

	uint faceNumber() const;
	uint faceContainerSize() const;
	uint deletedFaceNumber() const;

	uint addFace();

	template<typename... V>
	uint addFace(V... args) requires (sizeof...(args) >= 3);

	template<typename Iterator>
	uint addFace(Iterator begin, Iterator end);

	uint addFaces(uint n);
	void reserveFaces(uint n);
	void compactFaces();

	void deleteFace(uint i);
	void deleteFace(const FaceType* f);

	uint             faceIndexIfCompact(uint id) const;
	std::vector<uint> faceCompactIndices() const;

	void updateFaceIndices(const std::vector<uint>& newIndices);

	FaceIterator      faceBegin(bool jumpDeleted = true);
	FaceIterator      faceEnd();
	ConstFaceIterator faceBegin(bool jumpDeleted = true) const;
	ConstFaceIterator faceEnd() const;
	auto              faces(bool jumpDeleted = true);
	auto              faces(bool jumpDeleted = true) const;

	void enableAllPerFaceOptionalComponents();
	void disableAllPerFaceOptionalComponents();

	// AdjacentEdges
	bool isPerFaceAdjacentEdgesEnabled() const requires face::HasOptionalAdjacentEdges<T>;
	void enablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>;
	void disablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>;

	// AdjacentFaces
	bool isPerFaceAdjacentFacesEnabled() const requires face::HasOptionalAdjacentFaces<T>;
	void enablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>;
	void disablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>;

	// Color
	bool isPerFaceColorEnabled() const requires face::HasOptionalColor<T>;
	void enablePerFaceColor() requires face::HasOptionalColor<T>;
	void disablePerFaceColor() requires face::HasOptionalColor<T>;

	// Mark
	bool isPerFaceMarkEnabled() const requires face::HasOptionalMark<T>;
	void enablePerFaceMark() requires face::HasOptionalMark<T>;
	void disablePerFaceMark() requires face::HasOptionalMark<T>;

	// Normal
	bool isPerFaceNormalEnabled() const requires face::HasOptionalNormal<T>;
	void enablePerFaceNormal() requires face::HasOptionalNormal<T>;
	void disablePerFaceNormal() requires face::HasOptionalNormal<T>;

	// PrincipalCurvature
	bool isPerFacePrincipalCurvatureEnabled() const requires face::HasOptionalPrincipalCurvature<T>;
	void enablePerFacePrincipalCurvature() requires face::HasOptionalPrincipalCurvature<T>;
	void disablePerFacePrincipalCurvature() requires face::HasOptionalPrincipalCurvature<T>;

	// Quality
	bool isPerFaceQualityEnabled() const requires face::HasOptionalQuality<T>;
	void enablePerFaceQuality() requires face::HasOptionalQuality<T>;
	void disablePerFaceQuality() requires face::HasOptionalQuality<T>;

	// WedgeColors
	bool isPerFaceWedgeColorsEnabled() const requires face::HasOptionalWedgeColors<T>;
	void enablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>;
	void disablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>;

	// WedgeTexCoords
	bool isPerFaceWedgeTexCoordsEnabled() const requires face::HasOptionalWedgeTexCoords<T>;
	void enablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>;
	void disablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>;

	// Custom Components
	bool hasPerFaceCustomComponent(const std::string& name) const
		requires face::HasCustomComponents<T>;

	std::vector<std::string> perFaceCustomComponentNames() const
		requires face::HasCustomComponents<T>;

	template<typename K>
	bool isPerFaceCustomComponentOfType(const std::string& name) const
		requires face::HasCustomComponents<T>;

	std::type_index perFaceCustomComponentType(const std::string& name) const
		requires face::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> perFaceCustomComponentNamesOfType() const
		requires face::HasCustomComponents<T>;

	template<typename K>
	void addPerFaceCustomComponent(const std::string& name)
		requires face::HasCustomComponents<T>;

	void deletePerFaceCustomComponent(const std::string& name)
		requires face::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> perFaceCustomComponentVectorHandle(const std::string& name)
		requires face::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K>
	perFaceCustomComponentVectorHandle(const std::string& name) const
		requires face::HasCustomComponents<T>;

protected:
	template<typename OthMesh>
	void manageImportTriFromPoly(const OthMesh& m);

private:
	void addFaceHelper(T& f);

	template<typename... V>
	void addFaceHelper(T& f, typename T::VertexType* v, V... args);

	template<typename... V>
	void addFaceHelper(T& f, uint vid, V... args);

	template<typename MFaceType, typename VertexType, typename MVertexType>
	static void importTriPointersHelper(
		FaceType&                f,
		const MFaceType&         mf,
		VertexType*              base,
		const MVertexType*       mvbase,
		const std::vector<uint>& tris,
		uint                     basetri);
};

} // namespace vcl::mesh

#include "face_container.cpp"

#endif // VCL_MESH_CONTAINER_FACE_CONTAINER_H
