/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_TRIANGLE_BIT_FLAGS_H
#define MGP_MESH_FACE_TRIANGLE_BIT_FLAGS_H

#include "../common/bit_flags.h"

namespace mgp::face {

/**
 * @brief The TriangleBitFlags class represents a collection of 32 bits that will be part of a
 * Triangle of a Mesh. For othe components, please check for the more general BitFlags class.
 *
 * The bits have the following meaning: (first 3 bits inherited from BitFlags)
 * - 0:  deleted: if the current Triangle has been deleted
 * - 1:  selected: if the current Triangle has been selected
 * - 2:  edge border 0: if the current Triangle has his Edge with id 0 on border
 * - 3:  edge border 1: if the current Triangle has his Edge with id 1 on border
 * - 4:  edge border 2: if the current Triangle has his Edge with id 2 on border
 * - 5:  edge selected 0: if the current Triangle has his Edge with id 0 selected
 * - 6:  edge selected 1: if the current Triangle has his Edge with id 1 selected
 * - 7:  edge selected 2: if the current Triangle has his Edge with id 2 selected
 * - 8:  edge faux 0: if the current Triangle has his Edge with id 0 marked as faux
 * - 9:  edge faux 1: if the current Triangle has his Edge with id 1 marked as faux
 * - 10: edge faux 2: if the current Triangle has his Edge with id 2 marked as faux
 * - from 11 to 31: user bits that can have custom meanings to the user
 */
class TriangleBitFlags : public common::BitFlags
{
public:
	// member fuction that hide base members (to use the FIRST_USER_BIT value set here)
	bool userBitFlag(unsigned int bit) const;
	void setUserBit(unsigned int bit);
	void clearUserBit(unsigned int bit);

	bool isEdgeOnBorder0() const;
	bool isEdgeOnBorder1() const;
	bool isEdgeOnBorder2() const;
	bool isEdgeOnBorder(unsigned int i) const;
	bool isAnyEdgeOnBorder() const;

	bool isEdgeSelected0() const;
	bool isEdgeSelected1() const;
	bool isEdgeSelected2() const;
	bool isEdgeSelected(unsigned int i) const;

	bool isEdgeFaux0() const;
	bool isEdgeFaux1() const;
	bool isEdgeFaux2() const;
	bool isEdgeFaux(unsigned int i) const;
	bool isAnyEdgeFaux() const;

	void setEdgeOnBorder0();
	void setEdgeOnBorder1();
	void setEdgeOnBorder2();
	void setEdgeOnBorder(unsigned int i);

	void setEdgeSelected0();
	void setEdgeSelected1();
	void setEdgeSelected2();
	void setEdgeSelected(unsigned int i);

	void setEdgeFaux0();
	void setEdgeFaux1();
	void setEdgeFaux2();
	void setEdgeFaux(unsigned int i);

	void clearEdgeOnBorder0();
	void clearEdgeOnBorder1();
	void clearEdgeOnBorder2();
	void clearEdgeOnBorder(unsigned int i);

	void clearEdgeSelected0();
	void clearEdgeSelected1();
	void clearEdgeSelected2();
	void clearEdgeSelected(unsigned int i);

	void clearEdgeFaux0();
	void clearEdgeFaux1();
	void clearEdgeFaux2();
	void clearEdgeFaux(unsigned int i);
	void clearAllEdgeFaux();

protected:
	// hide base class constant, 8 is the number of bits used by this class
	static const unsigned int FIRST_USER_BIT = common::BitFlags::FIRST_USER_BIT + 8;

	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		// BORDER0 is BORDER, inherited from superclass
		BORDER1 = 1 << (common::BitFlags::FIRST_USER_BIT),
		BORDER2 = 1 << (common::BitFlags::FIRST_USER_BIT + 1),
		// Edge selection
		EDGESEL0 = 1 << (common::BitFlags::FIRST_USER_BIT + 2),
		EDGESEL1 = 1 << (common::BitFlags::FIRST_USER_BIT + 3),
		EDGESEL2 = 1 << (common::BitFlags::FIRST_USER_BIT + 4),
		// Faux edges: when representing polygonal meshes on triangle meshes, some triangle edges
		// can be marked as "faux", meaning that they are internal on the polygon
		FAUX0 = 1 << (common::BitFlags::FIRST_USER_BIT + 5),
		FAUX1 = 1 << (common::BitFlags::FIRST_USER_BIT + 6),
		FAUX2 = 1 << (common::BitFlags::FIRST_USER_BIT + 7)
	};

private:
	// will use these members as isOnBorder0, setOnBorder0 and clearOnBorder0
	using common::BitFlags::isOnBorder;
	using common::BitFlags::setOnBorder;
	using common::BitFlags::clearOnBorder;
};

/**
 * Detector to check if a class has (inherits) TriangleBitFlags
 */

template<typename T>
using hasTriangleBitFlags_t = decltype(std::declval<T&>().isFaceEdgeFaux0());

template<typename T>
using hasTriangleBitFlags = typename detector<hasTriangleBitFlags_t, void, T>::type;

} // namespace mgp::face

#include "triangle_bit_flags.cpp"

#endif // MGP_MESH_FACE_TRIANGLE_BIT_FLAGS_H
