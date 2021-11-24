/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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
 * The bits have the follwing meaning: (first 3 bits inherited from BitFlags)
 * - 0: deleted
 * - 1: delected
 * - from 2 to 13: edge border: if the current Face has is i-th edge (i in [0, 11]) on border
 * - from 14 to 25: edge selection: if the current Face has is i-th edge (i in [0, 11]) selected
 * - from 26 to 28: edge faux: if the current Face has is i-th edge (i in [0, 2]) marked as faux
 * - from 29 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 3 user bits, that can be accessed using the member functions
 * - userBitFlag
 * - setUserBit
 * - clearUserBit
 * with position in the interval [0, 2].
 */
class PolygonBitFlags : public BitFlags
{
public:
	// member fuction that hide base members (to use the FIRST_USER_BIT value set here)
	bool userBitFlag(uint bit) const;
	void setUserBit(uint bit);
	void clearUserBit(uint bit);

	bool isEdgeOnBorder(uint i) const;
	bool isAnyEdgeOnBorder() const;

	bool isEdgeSelected(uint i) const;
	bool isAnyEdgeSelected() const;

	bool isEdgeFaux(uint i) const;
	bool isAnyEdgeFaux() const;

	void setEdgeOnBorder(uint i);

	void setEdgeSelected(uint i);

	void setEdgeFaux(uint i);

	void clearEdgeOnBorder(uint i);
	void clearAllEdgeOnBorder();

	void clearEdgeSelected(uint i);
	void clearAllEdgeSelected();

	void clearEdgeFaux(uint i);
	void clearAllEdgeFaux();

protected:
	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		// Edge border
		// BORDER0 is BORDER, inherited from superclass - bits [2, 13]
		// Edge selection
		EDGESEL0 = 1 << (BitFlags::FIRST_USER_BIT + 11), // bits [14, 25]
		// Faux edges, for portability with TriangleBits
		FAUX0 = 1 << (BitFlags::FIRST_USER_BIT + 23) // bits [26, 28]
	};

	// hide base class constant, 26 is the number of bits used by this class
	static const uint FIRST_USER_BIT = BitFlags::FIRST_USER_BIT + 26; // bits [29, 31]

private:
	// will use these members as isOnBorder0, setOnBorder0 and clearOnBorder0
	using BitFlags::clearOnBorder;
	using BitFlags::isOnBorder;
	using BitFlags::setOnBorder;
};

/**
 * Detector to check if a class has (inherits) PolygonBitFlags
 */

template<typename T>
using hasPolygonBitFlagsT = std::is_base_of<PolygonBitFlags, T>;

template<typename T>
bool constexpr hasPolygonBitFlags()
{
	return hasPolygonBitFlagsT<T>::value;
}

} // namespace vcl::comp

#include "polygon_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
