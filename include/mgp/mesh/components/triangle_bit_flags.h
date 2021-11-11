/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
#define MGP_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H

#include "bit_flags.h"

namespace mgp::comp {

/**
 * @brief The TriangleBitFlags class represents a collection of 32 bits that will be part of a
 * Triangle of a Mesh.
 *
 * The bits have the following meaning: (first 3 bits inherited from BitFlags)
 * - 0:  deleted: if the current Triangle has been deleted
 * - 1:  selected: if the current Triangle has been selected
 * - from 2 to 4: edge border: if the current Triangle has is i-th edge (i in [0, 2]) on border
 * - from 5 to 7: edge selection: if the current Triangle has is i-th edge (i in [0, 2]) selected
 * - from 8 to 10: edge faux: if the current Triangle has is i-th edge (i in [0, 2]) marked as faux
 * - from 11 to 31: user bits that can have custom meanings to the user
 *
 * This class provides 21 user bits, that can be accessed using the member functions
 * - userBitFlag
 * - setUserBit
 * - clearUserBit
 * with position in the interval [0, 20].
 */
class TriangleBitFlags : public BitFlags
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
		// BORDER0 is BORDER, inherited from superclass  bits [2, 4]
		// Edge selection
		EDGESEL0 = 1 << (BitFlags::FIRST_USER_BIT + 2), // bits [5, 7]
		// Faux edges: when representing polygonal meshes on triangle meshes, some triangle edges
		// can be marked as "faux", meaning that they are internal on the polygon
		FAUX0 = 1 << (BitFlags::FIRST_USER_BIT + 5) // bits [8, 10]
	};

	// hide base class constant, 8 is the number of bits used by this class
	static const unsigned int FIRST_USER_BIT = BitFlags::FIRST_USER_BIT + 8; // bits [11, 31]

private:
	// will use these members as isOnBorder0, setOnBorder0 and clearOnBorder0
	using BitFlags::isOnBorder;
	using BitFlags::setOnBorder;
	using BitFlags::clearOnBorder;
};

/**
 * Detector to check if a class has (inherits) TriangleBitFlags
 */

template <typename  T>
using hasTriangleBitFlagsT = std::is_base_of<TriangleBitFlags, T>;

template <typename  T>
bool constexpr hasTriangleBitFlags() {return hasTriangleBitFlagsT<T>::value;}

} // namespace mgp::comp

#include "triangle_bit_flags.cpp"

#endif // MGP_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
