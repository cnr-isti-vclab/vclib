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

#ifndef VCL_MESH_COMPONENTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_BIT_FLAGS_H

#include <vclib/concepts/mesh/components/bit_flags.h>
#include <vclib/space/bit_set.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The BitFlags component class represents a collection of 8 bits that will be part of an
 * Element (e.g. Vertex, Face, ...).
 *
 * This Component (or a specialization) is mandatory into every Element of the mesh.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Element has been deleted - read only
 * - 1: selected: if the current Element has been selected
 * - 2: border: if the current Element is on border
 * - 3: visited: if the current Element has been visited (useful for some visit algorithms)
 * - other: user bits that can have custom meanings to the user
 *
 * This class provides 4 user bits, that can be accessed using the member function userBit(uint i)
 * with position in the interval [0, 3].
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Vertex Element `v` with the BitFlags component, you'll be able to
 * access to this component member functions from `v`:
 *
 * @code{.cpp}
 * bool isD = v.deleted();
 * @endcode
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
class BitFlags : public Component<BitSet<char>, ElementType, OPT>
{
	using Base = Component<BitSet<char>, ElementType, OPT>;
	using ThisType = BitFlags<ElementType, OPT>;

	using FT = char; // FlagsType, the integral type used for the flags

public:
	using BitFlagsComponent = ThisType; // expose the type to allow access to this component

	/* Constructor and isEnabled */

	BitFlags();

	void init();

	bool isEnabled() const;

	/* Member functions */

	bool deleted() const;

	BitProxy<FT> selected();
	bool selected() const;

	BitProxy<FT> onBorder();
	bool onBorder() const;

	BitProxy<FT> visited();
	bool visited() const;

	bool userBit(uint bit) const;
	BitProxy<FT> userBit(uint bit);

	void resetBitFlags();

	void importFromVCGFlags(int f);
	int exportToVCGFlags() const;

protected:
	BitProxy<FT> deleted();

	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the flags, trough data (horizontal) or trough parent (vertical)
	BitSet<FT>& flags();
	BitSet<FT> flags() const;

	static const uint FIRST_USER_BIT = 4;
	static const uint N_USER_BITS = sizeof(FT) * 8 - FIRST_USER_BIT;

	// indices of the bits
	enum {
		DELETED  = 0, // bit 0
		SELECTED = 1, // bit 1
		BORDER   = 2, // bit 2
		VISITED  = 3  // bit 3
	};
};

} // namespace vcl::comp

#include "bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_BIT_FLAGS_H
