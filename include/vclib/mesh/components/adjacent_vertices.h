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
#include <vclib/iterators/view.h>

#include "internal/element_references.h"

namespace vcl::comp {

/**
 * @brief The AdjacentVertices class is a container of Vertex references. It is a component that
 * makes sense to use mostly on Vertex Elements. For Faces and Edges, see the VertexReferences
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
 */
template<typename Vertex, typename ElementType = void, bool optional = false>
class AdjacentVertices :
		public ReferencesComponentTriggerer<Vertex>,
		protected internal::ElementReferences<Vertex, -1, ElementType>
{
	using ThisType = AdjacentVertices<Vertex, ElementType, optional>;

	using Base = internal::ElementReferences<Vertex, -1, ElementType>;

public:
	using DataValueType = typename Base::DataValueType; // data that the component stores internally (or vertically)
	using AdjacentVerticesComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using AdjacentVertexType = Vertex;

	/* Iterator Types declaration */

	using AdjacentVertexIterator      = typename Base::Iterator;
	using ConstAdjacentVertexIterator = typename Base::ConstIterator;
	using AdjacentVertexView          = vcl::View<AdjacentVertexIterator>;
	using ConstAdjacentVertexView     = vcl::View<ConstAdjacentVertexIterator>;

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

	AdjacentVertexIterator           adjVertexBegin();
	AdjacentVertexIterator           adjVertexEnd();
	ConstAdjacentVertexIterator      adjVertexBegin() const;
	ConstAdjacentVertexIterator      adjVertexEnd() const;
	AdjacentVertexView               adjVertices();
	ConstAdjacentVertexView          adjVertices() const;

protected:
	void updateReferences(const Vertex* oldBase, const Vertex* newBase);

	void updateReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template <typename Element>
	void importFrom(const Element& e);

	template<typename Element, typename ElVType>
	void importReferencesFrom(const Element& e, Vertex* base, const ElVType* ebase);

private:
	template<typename Element, typename ElVType>
	void
	importRefsFrom(const Element& e, Vertex* base, const ElVType* ebase);
};

} // namespace vcl::comp

#include "adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
