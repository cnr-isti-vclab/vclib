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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include <vclib/concepts/mesh/components/adjacent_vertices.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The AdjacentVertices class is a container of Vertex pointers. It is a
 * component that makes sense to use mostly on Vertex Elements. For Faces and
 * Edges, see the VertexPointers component (which is similar, but has different
 * member function names).
 *
 * It is a random access container having dynamic size.
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentVertices
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjVerticesNumber();
 * @endcode
 *
 * @note With respect to the other components that store adjacencies, this
 * component does not have two template arguments:
 * - `N`, because the container of adjacent vertices is always dynamic (there
 *   are no contexts in which it would make sense to have a fixed size container
 *   of adjacent vertices);
 * - `TTVN`, because this argument is used on components that could be part of
 *   face elements, and this component does not apply to faces.
 *
 * @tparam Vertex: The type of the adjacent Vertex element.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Vertex, typename ElementType = void, bool OPT = false>
class AdjacentVertices :
		public PointersContainerComponent<
			ADJACENT_VERTICES,
			Vertex,
			-1,
			ElementType,
			OPT,
			false>
{
	using Base = PointersContainerComponent<
		ADJACENT_VERTICES,
		Vertex,
		-1,
		ElementType,
		OPT,
		false>;

public:
	/**
	 * @brief Expose the type of the Adjacent Vertex.
	 */
	using AdjacentVertexType = Vertex;

	/* Iterator Types declaration */

	using AdjacentVertexIterator      = typename Base::Iterator;
	using ConstAdjacentVertexIterator = typename Base::ConstIterator;

	/* Constructor and isAvailable */

	void init();

	bool isAvailable() const;

	/* Member functions */

	uint adjVerticesNumber() const;

	Vertex*&      adjVertex(uint i);
	const Vertex* adjVertex(uint i) const;
	Vertex*&      adjVertexMod(int i);
	const Vertex* adjVertexMod(int i) const;

	void setAdjVertex(Vertex* v, uint i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	AdjacentVertexIterator      findAdjVertex(const Vertex* v);
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const;

	uint indexOfAdjVertex(const Vertex* v) const;

	void resizeAdjVertices(uint n);
	void pushAdjVertex(Vertex* v);
	void insertAdjVertex(uint i, Vertex* v);
	void eraseAdjVertex(uint i);
	void clearAdjVertices();

	/* Iterator Member functions */

	AdjacentVertexIterator            adjVertexBegin();
	AdjacentVertexIterator            adjVertexEnd();
	ConstAdjacentVertexIterator       adjVertexBegin() const;
	ConstAdjacentVertexIterator       adjVertexEnd() const;
	View<AdjacentVertexIterator>      adjVertices();
	View<ConstAdjacentVertexIterator> adjVertices() const;

	// dummy member to discriminate between AdjacentVertices and
	// VertexHalfEdgePointers
	void __adjacentVertices() const {}

protected:
	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// PointersContainerComponent interface functions
	template<typename Element, typename ElVType>
	void importPointersFrom(
		const Element& e,
		Vertex*        base,
		const ElVType* ebase);

	void updatePointers(const Vertex* oldBase, const Vertex* newBase);

	void updatePointers(
		const Vertex*            base,
		const std::vector<uint>& newIndices);

private:
	template<typename Element, typename ElVType>
	void importPtrsFrom(const Element& e, Vertex* base, const ElVType* ebase);
};

/* Detector function to check if a class has AdjacentVertices available */

bool isAdjacentVerticesAvailableOn(const ElementConcept auto& element);

} // namespace vcl::comp

#include "adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
