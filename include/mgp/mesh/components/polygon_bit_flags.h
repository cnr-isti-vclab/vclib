/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
#define MGP_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H

#include "bit_flags.h"

namespace mgp::comp {

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
	bool userBitFlag(unsigned int bit) const;
	void setUserBit(unsigned int bit);
	void clearUserBit(unsigned int bit);

	bool isEdgeOnBorder(unsigned int i) const;
	bool isAnyEdgeOnBorder() const;

	bool isEdgeSelected(unsigned int i) const;
	bool isAnyEdgeSelected() const;

	bool isEdgeFaux(unsigned int i) const;
	bool isAnyEdgeFaux() const;

	void setEdgeOnBorder(unsigned int i);

	void setEdgeSelected(unsigned int i);

	void setEdgeFaux(unsigned int i);

	void clearEdgeOnBorder(unsigned int i);
	void clearAllEdgeOnBorder();

	void clearEdgeSelected(unsigned int i);
	void clearAllEdgeSelected();

	void clearEdgeFaux(unsigned int i);
	void clearAllEdgeFaux();

protected:
	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		// Edge border
		// BORDER0 is BORDER, inherited from superclass - bits [2, 13]
		// Edge selection
		EDGESEL0  = 1 << (BitFlags::FIRST_USER_BIT + 11), // bits [14, 25]
		// Faux edges, for portability with TriangleBits
		FAUX0 = 1 << (BitFlags::FIRST_USER_BIT + 23) // bits [26, 28]
	};

	// hide base class constant, 26 is the number of bits used by this class
	static const unsigned int FIRST_USER_BIT = BitFlags::FIRST_USER_BIT + 26; // bits [29, 31]

private:
	// will use these members as isOnBorder0, setOnBorder0 and clearOnBorder0
	using BitFlags::isOnBorder;
	using BitFlags::setOnBorder;
	using BitFlags::clearOnBorder;
};

}

#include "polygon_bit_flags.cpp"

#endif // MGP_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
