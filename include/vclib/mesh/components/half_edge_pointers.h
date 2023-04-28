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

#ifndef VCL_MESH_COMPONENTS_HALF_EDGE_POINTERS_H
#define VCL_MESH_COMPONENTS_HALF_EDGE_POINTERS_H

#include <vector>

#include <vclib/concepts/mesh/components/half_edge_pointers.h>

#include "internal/component_data.h"

namespace vcl::comp {

/**
 * @brief The HalfEdgePointers class
 *
 * @ingroup components
 */
template<
	typename HalfEdge,
	typename Vertex,
	typename Face,
	typename ElementType = void,
	bool optional        = false>
class HalfEdgePointers :
		public PointersComponentTriggerer<HalfEdge>,
		public PointersComponentTriggerer<Vertex>,
		public PointersComponentTriggerer<Face>
{
	using ThisType = HalfEdgePointers<HalfEdge, Vertex, Face, ElementType, optional>;

	struct HEPData {
		HalfEdge* n; // next half edge
		HalfEdge* p; // prev half edge
		HalfEdge* t; // twin half edge
		Vertex*   v; // from vertex
		Face*     f; // incident face
	};
public:
	using DataValueType = HEPData; // data that the component stores internally (or vertically)
	using HalfEdgePointersComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using HalfEdgeType = HalfEdge;
	using VertexType   = Vertex;
	using FaceType     = Face;

	/* Constructor and isEnabled */

	HalfEdgePointers();

	void init();

	bool isEnabled();

	/* Member functions */

	const HalfEdge* next() const;
	HalfEdge*&      next();

	const HalfEdge* prev() const;
	HalfEdge*&      prev();

	const HalfEdge* twin() const;
	HalfEdge*&      twin();

	const Vertex* fromVertex() const;
	Vertex*&      fromVertex();

	const Vertex* toVertex() const;
	Vertex*&      toVertex();

	const Vertex* vertex(uint i) const;
	Vertex*&      vertex(uint i);

	const Face* face() const;
	Face*&      face();

protected:
	void updateReferences(const HalfEdge* oldBase, const HalfEdge* newBase);
	void updateReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	void updateReferences(const Face* oldBase, const Face* newBase);
	void updateReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

	void updateReferences(const Vertex* oldBase, const Vertex* newBase);
	void updateReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template<typename Element>
	void importFrom(const Element& e);

	template<typename HE, typename HEType>
	void importReferencesFrom(const HE& e, HalfEdge* base, const HEType* ebase);

	template<typename HE, typename VType>
	void importReferencesFrom(const HE& e, Vertex* base, const VType* ebase);

	template<typename HE, typename FType>
	void importReferencesFrom(const HE& e, Face* base, const FType* ebase);

private:
	// members that allow to access the data, trough data (horizontal) or trough parent (vertical)
	HalfEdge*& n(); // next half edge
	const HalfEdge* n() const;
	HalfEdge*& p(); // prev half edge
	const HalfEdge* p() const;
	HalfEdge*& t(); // twin half edge
	const HalfEdge* t() const;
	Vertex*&   v(); // from vertex
	const Vertex*   v() const;
	Face*&     f(); // incident face
	const Face*     f() const;

	internal::ComponentData<HEPData, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "half_edge_pointers.cpp"

#endif // VCL_MESH_COMPONENTS_HALF_EDGE_POINTERS_H
