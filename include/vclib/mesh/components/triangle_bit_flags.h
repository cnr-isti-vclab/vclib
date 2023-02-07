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

#include "bit_flags.h"

namespace vcl::comp {

/**
 * @brief The TriangleBitFlags class represents a collection of 32 bits that will be part of a
 * Triangle of a Mesh.
 *
 * The bits have the following meaning (first 3 bits inherited from BitFlags):
 * - 0:  deleted: if the current Triangle has been deleted
 * - 1:  selected: if the current Triangle has been selected
 * - from 2 to 4: edge border: if the current Triangle has is i-th edge (i in [0, 2]) on border
 * - from 5 to 7: edge selection: if the current Triangle has is i-th edge (i in [0, 2]) selected
 * - from 8 to 10: edge faux: if the current Triangle has is i-th edge (i in [0, 2]) marked as faux
 * - from 11 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 21 user bits, that can be accessed using the member functions
 * - `userBitFlag`
 * - `setUserBit`
 * - `unsetUserBit`
 *
 * with position in the interval [0, 20].
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Face Element `f` that has the TriangleBitFlags component, you'll be
 * able to access to this component member functions from `f`:
 *
 * @code{.cpp}
 * v.isAnyEdgeOnBorder();
 * @endcode
 */
template<typename ElementType = void, bool horizontal = true, bool optional = false>
class TriangleBitFlags : public BitFlags<ElementType, horizontal, optional>
{
	using Base = BitFlags<ElementType, horizontal, optional>;
	using ThisType = TriangleBitFlags<ElementType, horizontal, optional>;
public:
	using BitFlagsComponent = ThisType; // expose the type to allow access to this component
	// member fuction that hide base members (to use the FIRST_USER_BIT value set here)
	bool userBitFlag(uint bit) const;
	void setUserBit(uint bit);
	void unsetUserBit(uint bit);

	bool isEdgeOnBorder(uint i) const;
	bool isAnyEdgeOnBorder() const;
	bool isOnBorder() const;

	bool isEdgeSelected(uint i) const;
	bool isAnyEdgeSelected() const;

	bool isEdgeFaux(uint i) const;
	bool isAnyEdgeFaux() const;

	void setEdgeOnBorder(uint i);

	void setEdgeSelected(uint i);

	void setEdgeFaux(uint i);

	void unsetEdgeOnBorder(uint i);
	void unsetAllEdgesOnBorder();

	void unsetEdgeSelected(uint i);
	void unsetAllEdgesSelected();

	void unsetEdgeFaux(uint i);
	void unsetAllEdgeFaux();

	void importFromVCGFlags(int f);
	int exportToVCGFlags() const;
	
	// dummy member to discriminate between triangle and non-triangle bit flags
	void __triangleBitFlags() const {};

protected:
	template<typename Element>
	void importFrom(const Element& e);

protected:
	// values of the flags, used for flagValue, setFlag and unsetFlag member functions
	enum {
		// Edge border
		// BORDER0 is BORDER, inherited from superclass  bits [2, 4]
		// Edge selection
		EDGESEL0 = 1 << (Base::FIRST_USER_BIT + 2), // bits [5, 7]
		// Faux edges: when representing polygonal meshes on triangle meshes, some triangle edges
		// can be marked as "faux", meaning that they are internal on the polygon
		FAUX0 = 1 << (Base::FIRST_USER_BIT + 5) // bits [8, 10]
	};

	// hide base class constant, 8 is the number of bits used by this class
	static const uint FIRST_USER_BIT = Base::FIRST_USER_BIT + 8; // bits [11, 31]

private:
	// will use these members as isOnBorder0, setOnBorder0 and unsetOnBorder0
	using Base::isOnBorder;
	using Base::setOnBorder;
	using Base::unsetOnBorder;
};

} // namespace vcl::comp

#include "triangle_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
