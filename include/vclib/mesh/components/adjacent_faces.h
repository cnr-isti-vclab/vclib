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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_H

#include <vclib/concepts/mesh/components/adjacent_faces.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The AdjacentFaces class is a container of Face pointers. It could be
 * used by any Element to save adjacencies information (also the Face element
 * itself).
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of N (a negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentFaces
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjFacesNumber();
 * @endcode
 *
 * @note This component could be *Tied To Vertex Number*: it means that the size
 * of the container, if dynamic, will change automatically along the Vertex
 * Number of the Component. Check the `TTVN` template value on the
 * specialization of your component to check if it is tied to the Vertex Number.
 * For further details check the documentation of the @ref ContainerComponent
 * class.
 *
 * @tparam Face: The type of the adjacent Face element.
 * @tparam N: The size of the container, that will represent the number of
 * storable adjacent faces. If negative, the container is dynamic.
 * @tparam TTVN: If true, the size of the container will be tied to the Vertex
 * Number of the component (this is used mostly on Face elements).
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
	typename Face,
	int  N,
	bool TTVN,
	typename ElementType = void,
	bool OPT             = false>
class AdjacentFaces :
		public PointersContainerComponent<
			AdjacentFaces<Face, N, TTVN, ElementType, OPT>,
			ADJACENT_FACES,
			Face,
			N,
			ElementType,
			OPT,
			TTVN>
{
	using Base = PointersContainerComponent<
		AdjacentFaces<Face, N, TTVN, ElementType, OPT>,
		ADJACENT_FACES,
		Face,
		N,
		ElementType,
		OPT,
		TTVN>;

public:
	/**
	 * @brief Expose the type of the Adjacent Face.
	 */
	using AdjacentFaceType = Face;

	/* Iterator Types declaration */

	using AdjacentFaceIterator      = typename Base::Iterator;
	using ConstAdjacentFaceIterator = typename Base::ConstIterator;

	/**
	 * @brief Static size of the container. If the container is dynamic, this
	 * value will be negative and you should use the adjFacesNumber() member
	 * function.
	 */
	static const int ADJ_FACE_NUMBER = Base::SIZE;

	/* Member functions */

	uint adjFacesNumber() const;

	Face*&      adjFace(uint i);
	const Face* adjFace(uint i) const;
	Face*&      adjFaceMod(int i);
	const Face* adjFaceMod(int i) const;

	void setAdjFace(Face* f, uint i);
	void setAdjFaces(const std::vector<Face*>& list);

	bool containsAdjFace(const Face* f) const;

	AdjacentFaceIterator findAdjFace(const Face* f);
	ConstAdjacentFaceIterator findAdjFace(const Face* f) const;

	uint indexOfAdjFace(const Face* f) const;

	/* Member functions specific for vector adjacent faces */

	void resizeAdjFaces(uint n) requires (N < 0 && !TTVN);
	void pushAdjFace(Face* f) requires (N < 0 && !TTVN);
	void insertAdjFace(uint i, Face* f) requires (N < 0 && !TTVN);
	void eraseAdjFace(uint i) requires (N < 0 && !TTVN);
	void clearAdjFaces() requires (N < 0 && !TTVN);

	/* Iterator Member functions */

	AdjacentFaceIterator      adjFaceBegin();
	AdjacentFaceIterator      adjFaceEnd();
	ConstAdjacentFaceIterator adjFaceBegin() const;
	ConstAdjacentFaceIterator adjFaceEnd() const;
	auto                      adjFaces();
	auto                      adjFaces() const;

	// dummy member to discriminate between AdjacentFaces and
	// FaceHalfEdgePointers
	void __adjacentFaces() const {}

protected:
	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename Element, typename ElFType>
	void importPointersFrom(const Element& e, Face* base, const ElFType* ebase);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(Face* f = nullptr) requires (N < 0);
	void insert(uint i, Face* f = nullptr) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	template<typename Element, typename ElFType>
	void importPtrsFrom(const Element& e, Face* base, const ElFType* ebase);
};

/* Detector function to check if a class has AdjacentFaces available */

bool isAdjacentFacesAvailableOn(const ElementConcept auto& element);

} // namespace vcl::comp

#include "adjacent_faces.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_H
