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

#ifndef VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H

#include <vclib/concepts/mesh/components/bit_flags.h>
#include <vclib/space/bit_set.h>

#include "bases/container_component.h"

namespace vcl::comp {

/**
 * @brief The PolygonBitFlags class represents a collection of 8 bits plus 8
 * bits for each edge that will be part of a generic Polygonal Face of a Mesh.
 *
 * This is a specialization class of the BitFlags component, meaning that it can
 * be used in sostitution to that component. The difference is that this class
 * is meant to be used on Polygonal Faces, and adds bits with particular
 * meanings for polygons.
 *
 * This class also provides 3 flags for faux edges. These flags are added just
 * to make portable all the algorithms that use faux flags also for
 * PolygonMeshes. However, these flags should be used only when the mesh is a
 * Triangle mesh, that is when each face has vertexNumber() == 3.
 *
 * The bits have the follwing meaning:
 * - 0: deleted: if the current Polygon has been deleted - read only
 * - 1: selected: if the current Polygon has been selected
 * - 2: visited: if the current Polygon has been visited (useful for some visit
 *               algorithms)
 * - from 3 to 5: edge faux: if the current Face has is i-th edge (i in [0, 2])
 *                marked as faux
 * - from 6 to 7: user bits that can have custom meanings to the user
 *
 * This class provides 2 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 1].
 *
 * Additionally, this class provides the following bits for each edge of the
 * Polygonal Face:
 * - 0: edge border: if the current Polygonal face has the i-th edge on border
 * - 1: edge selection: if  the current Polygonal face has the i-th edge
 *                      selected
 * - 2: edge visited: if the i-th edge of the current Polygonal face has been
 *                    visited
 * - from 3 to 7: user bits of the i-th edge that can have custo meanings to the
 *                user
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the PolygonBitFlags
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * v.isAnyEdgeOnBorder();
 * @endcode
 *
 * @note This component is *Tied To Vertex Number*: it means that the size of
 * the container, if dynamic, will change automatically along the Vertex Number
 * of the Component. For further details check the documentation of the @ref
 * ContainerComponent class.
 *
 * @tparam N: The size of the container, that will represent the number of
 * storable bit flags for each edge of the Polygon. If negative, the container
 * is dynamic and tied to the vertex number.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<int N, typename ElementType = void, bool OPT = false>
class PolygonBitFlags :
		public ContainerComponent<
			BIT_FLAGS,
			BitSet<char>,
			N,
			BitSet<char>,
			ElementType,
			OPT,
			true>
{
	using FT = char; // FlagsType, the integral type used for the flags

	using Base = ContainerComponent<
		BIT_FLAGS,
		BitSet<FT>,
		N,
		BitSet<FT>,
		ElementType,
		OPT,
		true>;

	static const uint FIRST_USER_BIT = 6;
	static const uint FIRST_EDGE_USER_BIT = 3;

public:
	/**
	 * @brief Static number of bits that can have custom meanings to the user
	 */
	static const uint USER_BITS_NUMBER = sizeof(FT) * 8 - FIRST_USER_BIT;

	/**
	 * @brief Static number of bits for each edge that can have custom meanings
	 * to the user
	 */
	static const uint EDGE_USER_BITS_NUMBER =
		sizeof(FT) * 8 - FIRST_EDGE_USER_BIT;

	/* Constructor and isAvailable */
	PolygonBitFlags();

	void init();

	bool isAvailable() const;

	/* Member functions */

	bool deleted() const;

	BitProxy<FT> selected();
	bool selected() const;

	BitProxy<FT> visited();
	bool visited() const;

	bool onBorder() const;

	BitProxy<FT> edgeOnBorder(uint i);
	bool edgeOnBorder(uint i) const;

	BitProxy<FT> edgeSelected(uint i);
	bool edgeSelected(uint i) const;

	BitProxy<FT> edgeVisited(uint i);
	bool edgeVisited(uint i) const;

	BitProxy<FT> edgeFaux(uint i);
	bool edgeFaux(uint i) const;

	bool userBit(uint bit) const;
	BitProxy<FT> userBit(uint bit);

	bool edgeUserBit(uint i, uint bit) const;
	BitProxy<FT> edgeUserBit(uint i, uint bit);

	void resetBitFlags();

	void importFromVCGFlags(int f);
	int exportToVCGFlags() const;
	
	// dummy member to discriminate between triangle and non-triangle bit flags
	void __polygonBitFlags() const {}

protected:
	BitProxy<FT> deletedBit();

	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(BitSet<FT> f = BitSet<FT>()) requires (N < 0);
	void insert(uint i, BitSet<FT> f = BitSet<FT>()) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	// members that allow to access the flags, trough data (horizontal) or
	// trough parent (vertical)
	BitSet<FT>& flags();
	const BitSet<FT>& flags() const;
	Vector<BitSet<FT>, -1>& edgeFlags();
	const Vector<BitSet<FT>, -1>& edgeFlags() const;

	// indices of the bits
	enum {
		DELETED  = 0, // bit 0
		SELECTED = 1, // bit 1
		VISITED  = 2, // bit 2
		// Faux edges, for portability with TriangleBits
		FAUX0 = 3 // bits [3, 5]
	};

	// indices of the bits used for each edge of the polygon
	enum {
		EDGEBORD = 0,
		EDGESEL  = 1,
		EDGEVIS  = 2
	};
};

} // namespace vcl::comp

#include "polygon_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
