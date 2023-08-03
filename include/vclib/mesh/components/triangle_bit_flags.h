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

#ifndef VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H

#include <vclib/concepts/mesh/components/bit_flags.h>
#include <vclib/space/bit_set.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The TriangleBitFlags class represents a collection of 16 bits that
 * will be part of a Triangle of a Mesh.
 *
 * This is a specialization class of the BitFlags component, meaning that it can
 * be used in sostitution to that component. The difference is that this class
 * is meant to be used on Triangle Faces, and adds bits with particular meanings
 * for triangles.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Triangle has been deleted - read only
 * - 1: selected: if the current Triangle has been selected
 * - 2: visited: if the current Triangle has been visited (useful for some visit
 *               algorithms)
 * - from 3 to 5: edge border: if the current Triangle has the i-th edge (i in
 *                             [0, 2]) on border
 * - from 6 to 8: edge selection: if the current Triangle has the i-th edge (i
 *                                in [0, 2]) selected
 * - from 9 to 11: edge faux: if the current Triangle has the i-th edge (i in
 *                            [0, 2]) marked as faux
 * - from 12 to 15: user bits that can have custom meanings to the user
 *
 * This class provides 4 user bits, that can be accessed using the member
 * function userBit(uint i) with position in the interval [0, 3].
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the TriangleBitFlags
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * v.edgeSelected(1) = true;
 * @endcode
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
class TriangleBitFlags :
		public Component<BIT_FLAGS, BitSet<short>, ElementType, OPT>
{
	using Base = Component<BIT_FLAGS, BitSet<short>, ElementType, OPT>;

	using FT = short; // FlagsType, the integral type used for the flags

	static const uint FIRST_USER_BIT = 12;

public:
	/**
	 * @brief Static number of bits that can have custom meanings to the user
	 */
	static const uint USER_BITS_NUMBER = sizeof(FT) * 8 - FIRST_USER_BIT;

	/* Constructor and isAvailable */
	TriangleBitFlags();

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

	BitProxy<FT> edgeFaux(uint i);
	bool edgeFaux(uint i) const;

	bool userBit(uint bit) const;
	BitProxy<FT> userBit(uint bit);

	void resetBitFlags();

	void importFromVCGFlags(int f);
	int exportToVCGFlags() const;
	
	// dummy member to discriminate between triangle and non-triangle bit flags
	void __triangleBitFlags() const {}

protected:
	BitProxy<FT> deletedBit();

	// Component interface functions
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the flags, trough data (horizontal) or
	// trough parent (vertical)
	BitSet<FT>& flags();
	BitSet<FT> flags() const;

	// indices of the bits
	enum {
		DELETED  = 0, // bit 0
		SELECTED = 1, // bit 1
		VISITED  = 2, // bit 2
		// Edge border
		BORDER0 = 3, // bits [3, 5]
		// Edge selection
		EDGESEL0 = 6, // bits [6, 8]
		// Faux edges: when representing polygonal meshes on triangle meshes,
		// some triangle edges can be marked as "faux", meaning that they are
		// internal on the polygon
		FAUX0 = 9 // bits [9, 11]
	};
};

} // namespace vcl::comp

#include "triangle_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
