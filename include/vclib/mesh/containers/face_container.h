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
#include <vclib/space/polygon.h>
#include <vclib/views/mesh.h>

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
	using FaceIterator      = Base::ElementIterator;
	using ConstFaceIterator = Base::ConstElementIterator;

	/**
	 * @brief Empty constructor that creates an empty container of Faces.
	 */
	FaceContainer() = default;

	/**
	 * @brief Returns a const reference of the Face at the i-th position in the
	 * Face Container of the Mesh, which will be the Face having index = i.
	 *
	 * This function does not perform any sanity check: if i is less than
	 * faceContainerSize(), this function will return a valid Face reference
	 * (note that the Face may have been flagged as deleted).
	 *
	 * @param[in] i: the index of the Face that will be returned.
	 */
	const FaceType& face(uint i) const { return Base::element(i); }

	/**
	 * @brief Returns a reference of the Face at the i-th position in the Face
	 * Container of the Mesh, which will be the Face having index = i.
	 *
	 * This function does not perform any sanity check: if i is less than
	 * faceContainerSize(), this function will return a valid Face reference
	 * (note that the Face may have been flagged as deleted).
	 *
	 * @param[in] i: the index of the Face that will be returned.
	 */
	FaceType& face(uint i) { return Base::element(i); }

	/**
	 * @brief Returns the number of **non-deleted** Faces contained in the Face
	 * container of the Mesh.
	 *
	 * If faceNumber() != faceContainerSize(), it means that there are some
	 * Faces that are flagged as deleted.
	 *
	 * @return the number of non-deleted Faces of the Mesh.
	 */
	uint faceNumber() const { return Base::elementNumber(); }

	/**
	 * @brief Returns the number of Faces (also deleted) contained in the Face
	 * container of the Mesh.
	 *
	 * If faceNumber() != faceContainerSize(), it means that there are some
	 * Faces that are flagged as deleted.
	 *
	 * @return the number of all the Faces contained in the Mesh.
	 */
	uint faceContainerSize() const { return Base::elementContainerSize(); }

	/**
	 * @brief Returns the number of deleted Faces in the Face container, that is
	 * faceContainerSize() - faceNumber().
	 *
	 * @return The number of deleted Faces in the container.
	 */
	uint deletedFaceNumber() const { return Base::deletedElementNumber(); }

	/**
	 * @brief Add a Face to the container, returning its index.
	 *
	 * If the call of this function will cause a reallocation of the
	 * FaceContainer, the function will automatically take care of updating
	 * all the Face pointers contained in the Mesh.
	 *
	 * @return the id of the newly added Face.
	 */
	uint addFace() { return Base::addElement(); }

	template<typename... V>
	uint addFace(V... args) requires (sizeof...(args) >= 3)
	{
		uint  fid = addFace();
		Face& f   = face(fid);

		constexpr uint n = sizeof...(args);

		if constexpr (T::VERTEX_NUMBER < 0) {
			f.resizeVertices(n);
		}
		else {
			static_assert(
				n == T::VERTEX_NUMBER,
				"Wrong number of vertices in Mesh::addFace.");
		}

		addFaceHelper(f, args...);
		return fid;
	}

	// TODO requires on iterated type - must be Vertex* or int
	template<typename Iterator>
	uint addFace(Iterator begin, Iterator end)
	{
		if (begin == end) return UINT_NULL;
		uint n = std::distance(begin, end);

		uint fid = UINT_NULL;

		assert(n >= 3);
		if (n < 3) return UINT_NULL;

		if constexpr (T::VERTEX_NUMBER < 0) {
			fid = addFace();
			face(fid).resizeVertices(n);
		}
		else {
			assert(n == T::VERTEX_NUMBER);
			if (n == T::VERTEX_NUMBER)
				fid = addFace();
		}

		if (fid != UINT_NULL) {
			Face& f = face(fid);

			unsigned int i = 0;
			for (Iterator it = begin; it != end; ++it) {
				if constexpr (std::integral<typename Iterator::value_type>)
					f.vertex(i) = &Base::parentMesh->vertex(*it);
				else
					f.vertex(i) = *it;
				++i;
			}
		}
		return fid;
	}

	// TODO addFace function that takes a range

	/**
	 * @brief Add an arbitrary number of n Faces, returning the id of the first
	 * added Face.
	 *
	 * This means that, if you want to add 5 Faces and this member function
	 * returns 4, the added Faces will have id from 4 to id 8 included.
	 *
	 * If the call of this function will cause a reallocation of the
	 * FaceContainer, the function will automatically take care of updating all
	 * the Face pointers contained in the Mesh.
	 *
	 * @param n: the number of Faces to add to the mesh.
	 * @return the id of the first added Face.
	 */
	uint addFaces(uint n) { return Base::addElements(n); }

	/**
	 * @brief Clears the Face container of the Mesh, deleting all the
	 * Faces.
	 *
	 * The contained faces are actually removed from the container, not only
	 * marked as deleted. Therefore, the container will have size 0
	 * (`faceContainerSize() == 0`) after the call of this function.
	 *
	 * @note This function does not cause a reallocation of the Face
	 * container.
	 *
	 * @warning Any pointer to Faces in the Mesh will be left unchanged, and
	 * therefore will point to invalid Faces. This means that, if you have a
	 * pointer to a Face and you call this function, you will have a dangling
	 * pointer.
	 */
	void clearFaces() { Base::clearElements(); }

	/**
	 * @brief Resizes the Face container to contain `n` Faces.
	 *
	 * If the new size is greater than the old one, new Faces are added to the
	 * container, and a reallocation may happen. If the new size is smaller than
	 * the old one, the container will keep its first non-deleted `n` Faces, and
	 * the remaining Faces are marked as deleted.
	 *
	 * If the call of this function will cause a reallocation of the Face
	 * container, the function will automatically take care of updating all the
	 * Face pointers contained in the Mesh.
	 *
	 * @warning The given size `n` is relative to the number of non-deleted
	 * Faces, not to the size of the Face container. For example, if you
	 * have a mesh with 10 Faces and faceContainerSize() == 20, calling
	 * resizeFaces(5) will not cause a reallocation of the container, but will
	 * mark as deleted the least 5 non-deleted Faces of the container. In the
	 * same scenario, calling resizeFaces(15) will result in a Face
	 * container having 15 Faces and faceContainerSize() == 25.
	 * The latest 5 Faces will be the newly added.
	 *
	 * @warning Any pointer to deleted Faces in the Mesh will be left
	 * unchanged, and therefore will point to invalid Faces. This means that
	 * if you call this member function with a lower number of Faces, you'll
	 * need to manually manage the pointers to the deleted Faces.
	 *
	 * @param[in] n: the new size of the Face container.
	 */
	void resizeFaces(uint n) { Base::resizeElements(n); }

	/**
	 * @brief Reserve a number of Faces in the container of Faces. This is
	 * useful when you know (or you have an idea) of how much Faces are going to
	 * add into a newly of existing mesh. Calling this function before any
	 * addFace() call will avoid unuseful reallocations of the container, saving
	 * execution time.
	 *
	 * The filosofy of this function is similar to the one of the
	 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
	 * function of the [std::vector
	 * class](https://en.cppreference.com/w/cpp/container/vector).
	 *
	 * If the call of this function will cause a reallocation of the Face
	 * container, the function will automatically take care of updating all the
	 * Face pointers contained in the Mesh.
	 *
	 * @param n: the new capacity of the Face container.
	 */
	void reserveFaces(uint n) { Base::reserveElements(n); }

	/**
	 * @brief Compacts the FaceContainer, removing all the Faces marked as
	 * deleted. Faces indices will change accordingly. The function will
	 * automatically take care of updating all the Face pointers contained in
	 * the Mesh.
	 */
	void compactFaces() { Base::compactElements(); }

	/**
	 * @brief Marks as deleted the Face with the given id.
	 *
	 * This member function does not perform any reallocation of the Faces: the
	 * deleted Faces will stay in the Face Container, but will be marked as
	 * deleted.
	 *
	 * Deleted Faces are automatically jumped by the iterators provided by the
	 * Face Container.
	 *
	 * @param[in] i: the id of the Face that will be marked as deleted.
	 */
	void deleteFace(uint i) { Base::deleteElement(i); }

	/**
	 * @brief Marks as deleted the given Face, before asserting that the Face
	 * belongs to this container.
	 *
	 * This member function does not perform any reallocation of the Faces: the
	 * deleted Faces will stay in the Face Container, but will be marked as
	 * deleted.
	 *
	 * Deleted Faces are automatically jumped by the iterators provided by the
	 * Face Container.
	 *
	 * @param[in] f: the pointer of the Face that will be marked as deleted.
	 */
	void deleteFace(const FaceType* f) { Base::deleteElement(f); }

	/**
	 * @brief This is an utility member function that returns the index of an
	 * element if the container would be compact, that is the number of
	 * non-deleted elements before the Face with the given index.
	 *
	 * Complexity: O(n), with n the number of Faces in the container.
	 *
	 * This function does not perform any sanity check on the given index.
	 *
	 * @param[in] i: the index of a Face of the container.
	 * @return The index that the Face with index i would have if this container
	 * would be compact.
	 */
	uint faceIndexIfCompact(uint i) const
	{
		return Base::elementIndexIfCompact(i);
	}

	/**
	 * @brief Returns a vector that tells, for each actual Face index, the
	 * new index that the Face would have in a compacted container. For each
	 * deleted Face index, the value of the vector will be UINT_NULL.
	 *
	 * This is useful if you need to know the indices of the Faces that they
	 * would have in a compact container, without considering the deleted ones.
	 *
	 * @return A vector containing, for each Face index, its index if the
	 * container would be compact.
	 */
	std::vector<uint> faceCompactIndices() const
	{
		return Base::elementCompactIndices();
	}

	/**
	 * @brief Updates all the indices and pointers of the Faces of this
	 * container that are stored in any container of the mesh, according to the
	 * mapping stored in the newIndices vector, that tells for each old Face
	 * index, the new Face index.
	 *
	 * This function is useful when delete some Faces, and you want to update
	 * the indices/pointers stored in all the containers of the mesh
	 * accordingly.
	 *
	 * E.g. Supposing you deleted a set of Faces, you can give to this function
	 * the vector telling, for each one of the old Face indices, the new Face
	 * index (or UINT_NULL if you want to leave it unreferenced). This function
	 * will update all the pointers stored in the mesh containers accordingly
	 * (if they store adjacencies to the Faces).
	 *
	 * @note This function *does not change the position of the Faces in this
	 * container*. It just updates the indices/pointers of the Faces stored in
	 * this or other containers.
	 *
	 * @param[in] newIndices: a vector that tells, for each old Face index, the
	 * new Face index. If the old Face must be left as unreferenced (setting
	 * `nullptr` to the pointers), the value of the vector must be UINT_NULL.
	 */
	void updateFaceIndices(const std::vector<uint>& newIndices)
	{
		Base::updateElementIndices(newIndices);
	}

	/**
	 * @brief Returns an iterator to the beginning of the container.
	 *
	 * The iterator is automatically initialized to jump deleted Faces of
	 * the container. You can change this option by calling this function with
	 * `jumpDeleted=false`.
	 *
	 * @param[in] jumpDeleted: boolean that tells if the iterator
	 * should jump deleted Faces.
	 * @return An iterator the the first Face of the container.
	 */
	FaceIterator faceBegin(bool jumpDeleted = true)
	{
		return Base::elementBegin(jumpDeleted);
	}

	/**
	 * @brief Returns an iterator to the end of the container.
	 * @return An iterator to the end of the container.
	 */
	FaceIterator faceEnd() { return Base::elementEnd(); }

	/**
	 * @brief Returns a const iterator to the beginning of the container.
	 *
	 * The iterator is automatically initialized to jump deleted Faces of
	 * the container. You can change this option by calling this function with
	 * `jumpDeleted=false`.
	 *
	 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
	 * deleted Faces.
	 * @return A const iterator the the first Face of the container.
	 */
	ConstFaceIterator faceBegin(bool jumpDeleted = true) const
	{
		return Base::elementBegin(jumpDeleted);
	}

	/**
	 * @brief Returns a const iterator to the end of the container.
	 * @return A const iterator to the end of the container.
	 */
	ConstFaceIterator faceEnd() const { return Base::elementEnd(); }

	/**
	 * @brief Returns a small view object that allows to iterate over the Faces
	 * of the containers, providing two member functions begin() and end().
	 *
	 * This member function is very useful when you want to iterate over the
	 * Faces using the C++ foreach syntax:
	 *
	 * @code{.cpp}
	 * for (Face& e : m.faces()){
	 *     // do something with this Face
	 * }
	 * @endcode
	 *
	 * The iterator used to iterate over Faces is automatically initialized
	 * to jump deleted Faces of the container. You can change this option by
	 * calling this function with `jumpDeleted=false`.
	 *
	 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
	 * deleted Faces.
	 * @return An object having begin() and end() function, allowing to iterate
	 * over the container.
	 */
	auto faces(bool jumpDeleted = true) { return Base::elements(jumpDeleted); }

	/**
	 * @brief Returns a small view object that allows to iterate over the Faces
	 * of the containers, providing two member functions begin() and end().
	 *
	 * This member function is very useful when you want to iterate over the
	 * Faces using the C++ foreach syntax:
	 *
	 * @code{.cpp}
	 * for (const Face& e : m.faces()){
	 *     // do something with this Face
	 * }
	 * @endcode
	 *
	 * The iterator used to iterate over Faces is automatically initialized
	 * to jump deleted Faces of the container. You can change this option by
	 * calling this function with `jumpDeleted=false`.
	 *
	 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
	 * deleted Faces.
	 * @return An object having begin() and end() function, allowing to iterate
	 * over the container.
	 */
	auto faces(bool jumpDeleted = true) const
	{
		return Base::elements(jumpDeleted);
	}

	/**
	 * @brief Enables all the optional components associated to the Face type
	 * contained in the FaceContainer.
	 */
	void enableAllPerFaceOptionalComponents()
	{
		Base::enableAllOptionalComponents();
	}

	/**
	 * @brief Disables all the optional components associated to the Face type
	 * contained in the FaceContainer.
	 */
	void disableAllPerFaceOptionalComponents()
	{
		Base::disableAllOptionalComponents();
	}

	// AdjacentEdges

	/**
	 * @brief Checks if the Face Optional AdjacentEdges is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalAdjacentEdges Component.
	 *
	 * @return true if the Optional AdjacentEdges is enabled, false otherwise.
	 */
	bool isPerFaceAdjacentEdgesEnabled() const
		requires face::HasOptionalAdjacentEdges<T>
	{
		return Base::template isOptionalComponentEnabled<
			typename T::AdjacentEdges>();
	}

	/**
	 * @brief Enable the Optional AdjacentEdges of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalAdjacentEdges Component.
	 *
	 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the
	 * adjacent edges number will be the same of the vertex number for each face
	 * of the container. This is because, for Faces, Adjacent Edges number is
	 * tied to the number of vertices.
	 */
	void enablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>
	{
		Base::template enableOptionalComponent<typename T::AdjacentEdges>();
	}

	/**
	 * @brief Disables the Optional AdjacentEdges of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalAdjacentEdges Component.
	 */
	void disablePerFaceAdjacentEdges()
		requires face::HasOptionalAdjacentEdges<T>
	{
		Base::template disableOptionalComponent<typename T::AdjacentEdges>();
	}

	// AdjacentFaces

	/**
	 * @brief Checks if the Face Optional AdjacentFaces is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalAdjacentFaces Component.
	 *
	 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
	 */
	bool isPerFaceAdjacentFacesEnabled() const
		requires face::HasOptionalAdjacentFaces<T>
	{
		return Base::template isOptionalComponentEnabled<
			typename T::AdjacentFaces>();
	}

	/**
	 * @brief Enable the Optional AdjacentFaces of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalAdjacentFaces Component.
	 *
	 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the
	 * adjacent faces number will be the same of the vertex number for each face
	 * of the container. This is because, for Faces, Adjacent Faces number is
	 * tied to the number of vertices.
	 */
	void enablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>
	{
		Base::template enableOptionalComponent<typename T::AdjacentFaces>();
	}

	/**
	 * @brief Disables the Optional AdjacentFaces of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalAdjacentFaces Component.
	 */
	void disablePerFaceAdjacentFaces()
		requires face::HasOptionalAdjacentFaces<T>
	{
		Base::template disableOptionalComponent<typename T::AdjacentFaces>();
	}

	// Color

	/**
	 * @brief Checks if the Face Optional Color is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalColor Component.
	 *
	 * @return true if the Optional Color is enabled, false otherwise.
	 */
	bool isPerFaceColorEnabled() const requires face::HasOptionalColor<T>
	{
		return Base::template isOptionalComponentEnabled<typename T::Color>();
	}

	/**
	 * @brief Enable the Optional Color of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalColor Component.
	 */
	void enablePerFaceColor() requires face::HasOptionalColor<T>
	{
		Base::template enableOptionalComponent<typename T::Color>();
	}

	/**
	 * @brief Disables the Optional Color of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalColor Component.
	 */
	void disablePerFaceColor() requires face::HasOptionalColor<T>
	{
		Base::template disableOptionalComponent<typename T::Color>();
	}

	// Mark

	/**
	 * @brief Checks if the Face Optional Mark is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalMark Component.
	 *
	 * @return true if the Optional Mark is enabled, false otherwise.
	 */
	bool isPerFaceMarkEnabled() const requires face::HasOptionalMark<T>
	{
		return Base::template isOptionalComponentEnabled<typename T::Mark>();
	}

	/**
	 * @brief Enable the Optional Mark of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalMark Component.
	 */
	void enablePerFaceMark() requires face::HasOptionalMark<T>
	{
		Base::template enableOptionalComponent<typename T::Mark>();
	}

	/**
	 * @brief Disables the Optional Mark of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalMark Component.
	 */
	void disablePerFaceMark() requires face::HasOptionalMark<T>
	{
		Base::template disableOptionalComponent<typename T::Mark>();
	}

	// Normal

	/**
	 * @brief Checks if the Face Optional Normal is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalNormal Component.
	 *
	 * @return true if the Optional Normal is enabled, false otherwise.
	 */
	bool isPerFaceNormalEnabled() const requires face::HasOptionalNormal<T>
	{
		return Base::template isOptionalComponentEnabled<typename T::Normal>();
	}

	/**
	 * @brief Enable the Optional Normal of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalNormal Component.
	 */
	void enablePerFaceNormal() requires face::HasOptionalNormal<T>
	{
		Base::template enableOptionalComponent<typename T::Normal>();
	}

	/**
	 * @brief Disables the Optional Normal of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalNormal Component.
	 */
	void disablePerFaceNormal() requires face::HasOptionalNormal<T>
	{
		Base::template disableOptionalComponent<typename T::Normal>();
	}

	// PrincipalCurvature

	/**
	 * @brief Checks if the Face Optional PrincipalCurvature is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalPrincipalCurvature Component.
	 *
	 * @return true if the Optional PrincipalCurvature is enabled, false
	 * otherwise.
	 */
	bool isPerFacePrincipalCurvatureEnabled() const
		requires face::HasOptionalPrincipalCurvature<T>
	{
		return Base::template isOptionalComponentEnabled<
			typename T::PrincipalCurvature>();
	}

	/**
	 * @brief Enable the Optional PrincipalCurvature of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalPrincipalCurvature Component.
	 */
	void enablePerFacePrincipalCurvature()
		requires face::HasOptionalPrincipalCurvature<T>
	{
		Base::template enableOptionalComponent<
			typename T::PrincipalCurvature>();
	}

	/**
	 * @brief Disables the Optional PrincipalCurvature of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalPrincipalCurvature Component.
	 */
	void disablePerFacePrincipalCurvature()
		requires face::HasOptionalPrincipalCurvature<T>
	{
		Base::template disableOptionalComponent<
			typename T::PrincipalCurvature>();
	}

	// Quality

	/**
	 * @brief Checks if the Face Optional Quality is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalQuality Component.
	 *
	 * @return true if the Optional Quality is enabled, false otherwise.
	 */
	bool isPerFaceQualityEnabled() const requires face::HasOptionalQuality<T>
	{
		return Base::template isOptionalComponentEnabled<typename T::Quality>();
	}

	/**
	 * @brief Enable the Optional Quality of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalQuality Component.
	 */
	void enablePerFaceQuality() requires face::HasOptionalQuality<T>
	{
		Base::template enableOptionalComponent<typename T::Quality>();
	}

	/**
	 * @brief Disables the Optional Quality of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalQuality Component.
	 */
	void disablePerFaceQuality() requires face::HasOptionalQuality<T>
	{
		Base::template disableOptionalComponent<typename T::Quality>();
	}

	// WedgeColors

	/**
	 * @brief Checks if the Face Optional WedgeColors is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalWedgeColors Component.
	 *
	 * @return true if the Optional WedgeColors is enabled, false otherwise.
	 */
	bool isPerFaceWedgeColorsEnabled() const
		requires face::HasOptionalWedgeColors<T>
	{
		return Base::template isOptionalComponentEnabled<
			typename T::WedgeColors>();
	}

	/**
	 * @brief Enable the Optional WedgeColors of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalWedgeColors Component.
	 *
	 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the
	 * wedge colors number will be the same of the vertex number for each face
	 * of the container. This is because, for Faces, Wedge Colors number is tied
	 * to the number of vertices.
	 */
	void enablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>
	{
		Base::template enableOptionalComponent<typename T::WedgeColors>();
	}

	/**
	 * @brief Disables the Optional WedgeColors of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalWedgeColors Component.
	 */
	void disablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>
	{
		Base::template disableOptionalComponent<typename T::WedgeColors>();
	}

	// WedgeTexCoords

	/**
	 * @brief Checks if the Face Optional WedgeTexCoords is enabled.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalWedgeTexCoords Component.
	 *
	 * @return true if the Optional WedgeTexCoords is enabled, false otherwise.
	 */
	bool isPerFaceWedgeTexCoordsEnabled() const
		requires face::HasOptionalWedgeTexCoords<T>
	{
		return Base::template isOptionalComponentEnabled<
			typename T::WedgeTexCoords>();
	}

	/**
	 * @brief Enable the Optional WedgeTexCoords of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalWedgeTexCoords Component.
	 *
	 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the
	 * wedge texcoord number will be the same of the vertex number for each face
	 * of the container. This is because, for Faces, Wedge TexCoords number is
	 * tied to the number of vertices.
	 */
	void enablePerFaceWedgeTexCoords()
		requires face::HasOptionalWedgeTexCoords<T>
	{
		Base::template enableOptionalComponent<typename T::WedgeTexCoords>();
	}

	/**
	 * @brief Disables the Optional WedgeTexCoords of the Face.
	 *
	 * @note This function is available only if the Face Element has the
	 * OptionalWedgeTexCoords Component.
	 */
	void disablePerFaceWedgeTexCoords()
		requires face::HasOptionalWedgeTexCoords<T>
	{
		Base::template disableOptionalComponent<typename T::WedgeTexCoords>();
	}

	// Custom Components

	/**
	 * @brief Checks if Faces have a custom component with the given name.
	 *
	 * This function does not take into account the type of the custom
	 * component.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @return `true` if the Face Element has a custom component with the given
	 * name.
	 */
	bool hasPerFaceCustomComponent(const std::string& name) const
		requires face::HasCustomComponents<T>
	{
		return Base::hasElemCustomComponent(name);
	}

	/**
	 * @brief Returns a vector containing all the names of the custom components
	 * of any type associated to the Face Element.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @return A vector of strings representing all the names of the custom
	 * components.
	 */
	std::vector<std::string> perFaceCustomComponentNames() const
		requires face::HasCustomComponents<T>
	{
		return Base::elemCustomComponentNames();
	}

	/**
	 * @brief Checks if the custom component of the Face Element having the
	 * given name has the same type of the given template argument type of this
	 * function.
	 *
	 * For example, the following code checks if the component called `cc` is of
	 * type `double`:
	 * @code{.cpp}
	 * if (m.isPerFaceCustomComponentOfType<double>("cc")) {
	 *   ...
	 * }
	 * @endcode
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @tparam K: the type of the custom component to check.
	 *
	 * @param[in] name: the name of the custom component to check.
	 * @throws std::out_of_range if no custom component of the given name was
	 * found.
	 *
	 * @return `true` if the custom component is of the same type of the
	 * template argument.
	 */
	template<typename K>
	bool isPerFaceCustomComponentOfType(const std::string& name) const
		requires face::HasCustomComponents<T>
	{
		return Base::template isElemCustomComponentOfType<K>(name);
	}

	/**
	 * @brief Returns the std::type_index of the custom component of the Face
	 * Element having the given input name.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @param[in] name: the name of the custom component to get the
	 * std::type_index from.
	 * @throws std::out_of_range if no custom component of the given name was
	 * found.
	 *
	 * @return The std::type_index of the custom component having the given
	 * input name.
	 */
	std::type_index perFaceCustomComponentType(const std::string& name) const
		requires face::HasCustomComponents<T>
	{
		return Base::elemComponentType(name);
	}

	/**
	 * @brief Returns a vector containing all the names of the custom components
	 * associated to the Edge Element having the same type of the given template
	 * argument type of this function.
	 *
	 * For example, the following code gets a vector containing all the custom
	 * components of type `double`:
	 * @code{.cpp}
	 * std::vector<std::string> cdouble =
	 *     m.perFaceCustomComponentNamesOfType<double>();
	 * @endcode
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @tparam K: the type of the custom component names.
	 * @return A vector of strings representing the names of the custom
	 * components of a given type.
	 */
	template<typename K>
	std::vector<std::string> perFaceCustomComponentNamesOfType() const
		requires face::HasCustomComponents<T>
	{
		return Base::template elemCustomComponentNamesOfType<K>();
	}

	/**
	 * @brief Adds a custom component of type K to the Face, having the given
	 * name.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @tparam K: the type of the custom component added to the Face.
	 * @param[in] name: the name of the custom component added to the Face.
	 */
	template<typename K>
	void addPerFaceCustomComponent(const std::string& name)
		requires face::HasCustomComponents<T>
	{
		Base::template addElemCustomComponent<K>(name);
	}

	/**
	 * @brief Deletes the custom component of the given name from the Face
	 * Element.
	 *
	 * The function does nothing if the custom component does not exists.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @param[in] name: the name of the custom component that will be removed
	 * from the Face.
	 */
	void deletePerFaceCustomComponent(const std::string& name)
		requires face::HasCustomComponents<T>
	{
		Base::deleteElemCustomComponent(name);
	}

	/**
	 * @brief Returns a vector handle to the given custom component. The handle
	 * can be used like a normal std::vector, but does not have access to the
	 * modifiers member functions (resize, push_back...). The handle contains
	 * **references** to the custom component, therefore you can modify the
	 * custom component by modifying the element of the handle vector normally.
	 * Since the handle stores references, there are no copies performed when
	 * calling this function.
	 *
	 * For example, assuming that the mesh has a Face custom component named
	 * `cc` of type int:
	 *
	 * @code{.cpp}
	 * auto handle = m.getPerFaceCustomComponentVectorHandle<int>("cc");
	 * for (Face& e : m.faces() {
	 *    handle[m.index(e)] = 5; // e.customComponent<int>("cc") == 5
	 *    assert(e.customComponent<int>("cc") == 5);
	 * }
	 * @endcode
	 *
	 * Using handles allows to access more efficiently to custom components
	 * rather accessing from an element object. However, note that references
	 * are binded to the container of the mesh.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @note Since the handle contains references, any operation that changes
	 * the size of the container could be destructive and invalidate the
	 * references contained in the handle.
	 *
	 * @tparam K: the type of the custom component on which return the handle.
	 *
	 * @param[in] name: name of the custom component on which return the handle.
	 * @throws std::out_of_range if no custom component of the given name was
	 * found.
	 *
	 * @return a vector handle that allows to access to the custom component.
	 */
	template<typename K>
	CustomComponentVectorHandle<K>
	perFaceCustomComponentVectorHandle(const std::string& name)
		requires face::HasCustomComponents<T>
	{
		return Base::template customComponentVectorHandle<K>(name);
	}

	/**
	 * @brief Returns a const vector handle to the given custom component. The
	 * handle can be used like a normal std::vector, but does not have access to
	 * the modifiers member functions (resize, push_back...). The handle
	 * contains
	 * **const references** to the custom component, therefore you can access to
	 * the custom component by accessing the element of the handle vector
	 * normally. Since the handle stores references, there are no copies
	 * performed when calling this function.
	 *
	 * For example, assuming that the mesh has a Face custom component named
	 * `cc` of type int:
	 *
	 * @code{.cpp}
	 * // access to the const handle by making const the template parameter:
	 * auto handle = m.perFaceCustomComponentVectorHandle<const int>("cc");
	 * int sum = 0;
	 * for (const Face& e : m.faces() {
	 *    sum += handle[m.index(e)];
	 *    // handle[m.index(e)] = 5; // not allowed, because the handle is const
	 * }
	 * @endcode
	 *
	 * Using handles allows to access more efficiently to custom components
	 * rather accessing from an element object. However, note that references
	 * are binded to the container of the mesh.
	 *
	 * @note This function is available only if the Face Element has the
	 * CustomComponents Component.
	 *
	 * @note Since the handle contains references, any operation that changes
	 * the size of the container could be destructive and invalidate the
	 * references contained in the handle.
	 *
	 * @tparam K: the type of the custom component on which return the handle.
	 * @param[in] name: name of the custom component on which return the handle.
	 *
	 * @return a const vector handle that allows to access to the custom
	 * component.
	 */
	template<typename K>
	ConstCustomComponentVectorHandle<K>
	perFaceCustomComponentVectorHandle(const std::string& name) const
		requires face::HasCustomComponents<T>
	{
		return Base::template customComponentVectorHandle<K>(name);
	}

protected:
	/**
	 * @brief This function manages the case where we try to import into a
	 * TriMesh a PolyMesh Faces have been already imported, but without vertex
	 * pointers and other components that depend on the number of vertices (e.g.
	 * wedges)
	 */
	template<typename OthMesh>
	void manageImportTriFromPoly(const OthMesh& m)
	{
		if constexpr (HasFaceContainer<OthMesh>) {
			using ParentMesh = Base::ParentMeshType;
			using VertexType = ParentMesh::VertexType;
			using MVertexType = OthMesh::VertexType;
			using MCoordType = MVertexType::CoordType;
			using MFaceType = OthMesh::FaceType;

			using VertexContainer = ParentMesh::VertexContainer;

			// if this is not a triangle mesh nor a polygon mesh (meaning that
			// we can't control the number of vertex pointers in this mesh), and
			// this mesh does not have the same number of vertex pointers of the
			// other, it means that we don't know how to convert these type of
			// meshes (e.g. we don't know how to convert a polygon mesh into a
			// quad mesh, or convert a quad mesh into a pentagonal mesh...)
			static_assert(
				!(FaceType::VERTEX_NUMBER != 3 && FaceType::VERTEX_NUMBER > 0 &&
				  FaceType::VERTEX_NUMBER != MFaceType::VERTEX_NUMBER),
				"Cannot import from that type of Mesh. Don't know how to "
				"convert "
				"faces.");

			// we need to manage conversion from poly or faces with cardinality
			// > 3 (e.g. quads) to triangle meshes. In this case, we triangulate
			// the polygon using the earcut algorithm.
			if constexpr (
				FaceType::VERTEX_NUMBER == 3 &&
				(MFaceType::VERTEX_NUMBER > 3 || MFaceType::VERTEX_NUMBER < 0))
			{
				VertexType* base = &Base::parentMesh->vertex(0);
				const MVertexType* mvbase = &m.vertex(0);

				for (const MFaceType& mf : m.faces()) {
					// if the current face has the same number of vertices of
					// this faces (3), then the vertex pointers have been
					// correctly imported from the import pointers function. The
					// import pointers function does nothing when importing from
					// a face with at least 4 vertices
					if (mf.vertexNumber() != FaceType::VERTEX_NUMBER) {
						// triangulate mf; the first triangle of the
						// triangulation will be this->face(m.index(mf)); the
						// other triangles will be added at the end of the
						// container
						std::vector<uint> tris = Polygon<MCoordType>::earCut(
							mf.vertices() | vcl::views::coords);
						FaceType& f = face(m.index(mf));
						importTriPointersHelper(f, mf, base, mvbase, tris, 0);

						// number of other faces to add
						uint nf = tris.size() / 3 - 1;
						uint fid = addFaces(nf);

						uint i = 3; // index that cycles into tris
						for (; fid < faceContainerSize(); ++fid) {
							FaceType& f = face(fid);
							importTriPointersHelper(
								f, mf, base, mvbase, tris, i);
							i+=3;
						}
					}
				}
			}
		}
	}

private:
	void addFaceHelper(T& f) { /* base case: no need to add vertices */ }

	template<typename... V>
	void addFaceHelper(T& f, typename T::VertexType* v, V... args)
	{
		// position on which add the vertex
		const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
		f.vertex(n)         = v; // set the vertex
		// set the remanining vertices, recursive variadics
		addFaceHelper(f, args...);
	}

	template<typename... V>
	void addFaceHelper(T& f, uint vid, V... args)
	{
		// position on which add the vertex
		const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
		f.vertex(n)         = &Base::parentMesh->vertex(vid); // set the vertex
		// set the remanining vertices, recursive variadics
		addFaceHelper(f, args...);
	}

	template<typename MFaceType, typename VertexType, typename MVertexType>
	static void importTriPointersHelper(
		FaceType&                f,
		const MFaceType&         mf,
		VertexType*              base,
		const MVertexType*       mvbase,
		const std::vector<uint>& tris,
		uint                     basetri)
	{
		f.importFrom(mf); // import all the components from mf
		for (uint i = basetri, j = 0; i < basetri+3; i++, j++) {
			f.vertex(j) = base + (mf.vertex(tris[i]) - mvbase);

			// wedge colors
			if constexpr (
				face::HasWedgeColors<FaceType> &&
				face::HasWedgeColors<MFaceType>)
			{
				if (comp::isWedgeColorsAvailableOn(f) &&
					comp::isWedgeColorsAvailableOn(mf))
				{
					f.wedgeColor(j) = mf.wedgeColor(tris[i]);
				}
			}

			// wedge texcoords
			if constexpr (
				face::HasWedgeTexCoords<FaceType> &&
				face::HasWedgeTexCoords<MFaceType>)
			{
				if (comp::isWedgeTexCoordsAvailableOn(f) &&
					comp::isWedgeTexCoordsAvailableOn(mf))
				{
					using ST = typename FaceType::WedgeTexCoordType::ScalarType;
					f.wedgeTexCoord(j) =
						mf.wedgeTexCoord(tris[i]).template cast<ST>();
				}
			}
		}
	}
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINER_FACE_CONTAINER_H
