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

#include "bit_flags.h"

namespace vcl::comp {

/**
 * @brief The PolygonBitFlags class represents a collection of 32 bits that will be part of a
 * generic Polygonal Face of a Mesh.
 *
 * Polygons that use this class for Bit flags can mark as border or as selected a maximum number of
 * 12 edges. If you need bigger polygons on your mesh, you should use the @todo BigPolygonBitFlags
 * class.
 *
 * This class also provides 3 flags for faux edges. These flags are added just to make portable all
 * the algorithms that use faux flags also for PolygonMeshes. However, these flags should be used
 * only when the mesh is a Triangle mesh, that is when each face has vertexNumber() == 3.
 *
 * The bits have the follwing meaning (first 3 bits inherited from BitFlags):
 * - 0: deleted: if the current Polygon has been deleted
 * - 1: selected: if the current Polygon has been selected
 * - from 2 to 13: edge border: if the current Face has is i-th edge (i in [0, 11]) on border
 * - from 14 to 25: edge selection: if the current Face has is i-th edge (i in [0, 11]) selected
 * - from 26 to 28: edge faux: if the current Face has is i-th edge (i in [0, 2]) marked as faux
 * - from 29 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 3 user bits, that can be accessed using the member functions
 * - `userBitFlag`
 * - `setUserBit`
 * - `unsetUserBit`
 *
 * with position in the interval [0, 2].
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Face Element `f` that has the PolygonBitFlags component, you'll be
 * able to access to this component member functions from `f`:
 *
 * @code{.cpp}
 * v.isAnyEdgeOnBorder();
 * @endcode
 *
 * @ingroup components
 */
template<typename ElementType = void, bool optional = false>
class PolygonBitFlags
{
	using ThisType = PolygonBitFlags<ElementType, optional>;
public:
	using DataValueType = BitSet<int>; // data that the component stores internally (or vertically)

	using BitFlagsComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	/* Constructor and isEnabled */
	PolygonBitFlags();

	void init();

	bool isEnabled() const;

	/* Member functions */

	bool deleted() const;

	BitProxy<int> selected();
	bool selected() const;

	bool onBorder() const;

	BitProxy<int> edgeOnBorder(uint i);
	bool edgeOnBorder(uint i) const;

	BitProxy<int> edgeSelected(uint i);
	bool edgeSelected(uint i) const;

	BitProxy<int> edgeFaux(uint i);
	bool edgeFaux(uint i) const;

	bool userBit(uint bit) const;
	BitProxy<int> userBit(uint bit);

	void resetBitFlags();

	void importFromVCGFlags(int f);
	int exportToVCGFlags() const;
	
	// dummy member to discriminate between triangle and non-triangle bit flags
	void __polygonBitFlags() const {}

protected:
	BitProxy<int> deleted();

	template<typename Element>
	void importFrom(const Element& e);

	// members that allow to access the flags, trough data (horizontal) or trough parent (vertical)
	BitSet<int>& flags();
	BitSet<int> flags() const;

	static const uint FIRST_USER_BIT = 30; // bits [29, 31]

	// indices of the bits, used for flagValue, setFlag and unsetFlag member functions
	enum {
		DELETED  = 0, // bit 0
		SELECTED = 1, // bit 1
		VISITED  = 2, // bit 2
		// Edge border
		BORDER0  = 3, // bits [3, 14]
		// Edge selection
		EDGESEL0 = 15, // bits [15, 26]
		// Faux edges, for portability with TriangleBits
		FAUX0 = 27 // bits [27, 29]
	};

private:
	// contians the actual data of the component, if the component is horizontal
	internal::ComponentData<DataValueType, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "polygon_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
