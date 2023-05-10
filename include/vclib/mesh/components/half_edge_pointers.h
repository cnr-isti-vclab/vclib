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

#include "bases/component.h"

namespace vcl::comp {

namespace internal {

template<typename HalfEdge, typename Vertex, typename Face>
struct HEPData {
	HalfEdge* n; // next half edge
	HalfEdge* p; // prev half edge
	HalfEdge* t; // twin half edge
	Vertex*   v; // from vertex
	Face*     f; // incident face
};

} // namespace vcl::comp::internal

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
		public Component<
			internal::HEPData<HalfEdge, Vertex, Face>,
			ElementType,
			optional,
			HalfEdge,
			Vertex,
			Face>
{
	using Base = Component<
		internal::HEPData<HalfEdge, Vertex, Face>,
		ElementType,
		optional,
		HalfEdge,
		Vertex,
		Face>;
	using ThisType = HalfEdgePointers<HalfEdge, Vertex, Face, ElementType, optional>;

public:
	using HalfEdgePointersComponent = ThisType; // expose the type to allow access to this component

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
	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename HE, typename HEType>
	void importPointersFrom(const HE& e, HalfEdge* base, const HEType* ebase);

	template<typename HE, typename VType>
	void importPointersFrom(const HE& e, Vertex* base, const VType* ebase);

	template<typename HE, typename FType>
	void importPointersFrom(const HE& e, Face* base, const FType* ebase);

	void updatePointers(const HalfEdge* oldBase, const HalfEdge* newBase);
	void updatePointersAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	void updatePointers(const Face* oldBase, const Face* newBase);
	void updatePointersAfterCompact(const Face* base, const std::vector<int>& newIndices);

	void updatePointers(const Vertex* oldBase, const Vertex* newBase);
	void updatePointersAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

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
};

} // namespace vcl::comp

#include "half_edge_pointers.cpp"

#endif // VCL_MESH_COMPONENTS_HALF_EDGE_POINTERS_H
