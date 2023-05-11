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
 * @brief The AdjacentVertices class is a container of Vertex pointers. It is a component that
 * makes sense to use mostly on Vertex Elements. For Faces and Edges, see the VertexPointers
 * component (which is similar, but has different member function names).
 *
 * It is a random access container having dynamic size.
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentVertices component, you'll be
 * able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * v.adjVerticesNumber();
 * @endcode
 *
 * @ingroup components
 */
template<typename Vertex, typename ElementType = void, bool OPTIONAL = false>
class AdjacentVertices : public PointersContainerComponent<Vertex, -1, ElementType, OPTIONAL, false>
{
	using ThisType = AdjacentVertices<Vertex, ElementType, OPTIONAL>;
	
	using Base = PointersContainerComponent<Vertex, -1, ElementType, OPTIONAL, false>;

public:
	using AdjacentVerticesComponent = ThisType; // expose the type to allow access to this component

	using AdjacentVertexType = Vertex;

	/* Iterator Types declaration */

	using AdjacentVertexIterator      = typename Base::Iterator;
	using ConstAdjacentVertexIterator = typename Base::ConstIterator;

	/* Constructor and isEnabled */

	void init();

	bool isEnabled() const;
	bool isAdjVerticesEnabled() const;

	/* Member functions */

	uint adjVerticesNumber() const;

	Vertex*&      adjVertex(uint i);
	const Vertex* adjVertex(uint i) const;
	Vertex*&      adjVertexMod(int i);
	const Vertex* adjVertexMod(int i) const;

	void setAdjVertex(Vertex* v, uint i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	AdjacentVertexIterator findAdjVertex(const Vertex* v);
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const;

	int indexOfAdjVertex(const Vertex* v) const;

	void resizeAdjVertices(uint n);
	void pushAdjVertex(Vertex* v);
	void insertAdjVertex(uint i, Vertex* v);
	void eraseAdjVertex(uint i);
	void clearAdjVertices();

	/* Iterator Member functions */

	AdjacentVertexIterator      adjVertexBegin();
	AdjacentVertexIterator      adjVertexEnd();
	ConstAdjacentVertexIterator adjVertexBegin() const;
	ConstAdjacentVertexIterator adjVertexEnd() const;
	auto                        adjVertices();
	auto                        adjVertices() const;

protected:
	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// PointersContainerComponent interface functions
	template<typename Element, typename ElVType>
	void importPointersFrom(const Element& e, Vertex* base, const ElVType* ebase);

	void updatePointers(const Vertex* oldBase, const Vertex* newBase);

	void updatePointersAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	// ContainerComponent interface functions

private:
	template<typename Element, typename ElVType>
	void importPtrsFrom(const Element& e, Vertex* base, const ElVType* ebase);
};

/* Detector function to check if a class has AdjacentVertices enabled */

template <typename T>
bool isAdjacentVerticesEnabledOn(const T& element);

} // namespace vcl::comp

#include "adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
