/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "../detection/bit_flags_detection.h"

namespace vcl::comp {

/**
 * @brief The BitFlags component class represents a collection of 32 bits that will be part of an
 * Element (e.g. Vertex, Face, ...).
 *
 * This Component (or a specialization) is mandatory into every Element of the mesh.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Element has been deleted
 * - 1: selected: if the current Element has been selected
 * - 2: border: if the current Element is on border
 * - other: user bits that can have custom meanings to the user
 *
 * This class provides 29 user bits, that can be accessed using the member functions
 * - `userBitFlag`
 * - `setUserBit`
 * - `unsetUserBit`
 *
 * with position in the interval [0, 28].
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Vertex Element `v` with the BitFlags component, you'll be able to
 * access to this component member functions from `v`:
 *
 * @code{.cpp}
 * v.isDeleted();
 * @endcode
 */
class BitFlags
{
public:
	bool isDeleted() const;
	bool isSelected() const;
	bool isOnBorder() const;
	bool userBitFlag(uint bit) const;

	void setSelected();
	void setOnBorder();
	void setUserBit(uint bit);

	void unsetAllFlags();
	void unsetSelected();
	void unsetOnBorder();
	void unsetUserBit(uint bit);

	void importFromVCGFlags(int f);
	int exportToVCGFlags() const;

protected:
	void setDeleted();
	void unsetDeleted();

	bool flagValue(uint flag) const;
	void setFlag(uint flag);
	void unsetFlag(uint flag);

	bool userBitFlag(uint bit, uint firstBit) const;
	void setUserBit(uint bit, uint firstBit);
	void unsetUserBit(uint bit, uint firstBit);

	template<typename Element>
	void importFrom(const Element& e);

	int flags = 0;

	static const uint FIRST_USER_BIT = 3;

	// values of the flags, used for flagValue, setFlag and unsetFlag member functions
	enum {
		DELETED  = 1 << 0, // bit 0
		SELECTED = 1 << 1, // bit 1
		BORDER   = 1 << 2  // bit 2
	};
};

} // namespace vcl::comp

#include "bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_BIT_FLAGS_H