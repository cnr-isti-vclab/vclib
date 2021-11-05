/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_POLYGON_BIT_FLAGS_H
#define MGP_MESH_PROPERTIES_POLYGON_BIT_FLAGS_H

#include "bit_flags.h"

namespace mgp::prop {

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
	bool isAnyEdgeSelected();

	void setEdgeOnBorder(unsigned int i);

	void setEdgeSelected(unsigned int i);

	void clearEdgeOnBorder(unsigned int i);
	void clearAllEdgeOnBorder();

	void clearEdgeSelected(unsigned int i);

protected:
	// hide base class constant, 23 is the number of bits used by this class
	static const unsigned int FIRST_USER_BIT = BitFlags::FIRST_USER_BIT + 23;

	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		// BORDER0 is BORDER, inherited from superclass
		BORDER1  = 1 << (BitFlags::FIRST_USER_BIT),
		BORDER2  = 1 << (BitFlags::FIRST_USER_BIT + 1),
		BORDER3  = 1 << (BitFlags::FIRST_USER_BIT + 2),
		BORDER4  = 1 << (BitFlags::FIRST_USER_BIT + 3),
		BORDER5  = 1 << (BitFlags::FIRST_USER_BIT + 4),
		BORDER6  = 1 << (BitFlags::FIRST_USER_BIT + 5),
		BORDER7  = 1 << (BitFlags::FIRST_USER_BIT + 6),
		BORDER8  = 1 << (BitFlags::FIRST_USER_BIT + 7),
		BORDER9  = 1 << (BitFlags::FIRST_USER_BIT + 8),
		BORDER10 = 1 << (BitFlags::FIRST_USER_BIT + 9),
		BORDER11 = 1 << (BitFlags::FIRST_USER_BIT + 10), // 12 bits for border
		// Edge selection
		EDGESEL0  = 1 << (BitFlags::FIRST_USER_BIT + 11),
		EDGESEL1  = 1 << (BitFlags::FIRST_USER_BIT + 12),
		EDGESEL2  = 1 << (BitFlags::FIRST_USER_BIT + 13),
		EDGESEL3  = 1 << (BitFlags::FIRST_USER_BIT + 14),
		EDGESEL4  = 1 << (BitFlags::FIRST_USER_BIT + 15),
		EDGESEL5  = 1 << (BitFlags::FIRST_USER_BIT + 16),
		EDGESEL6  = 1 << (BitFlags::FIRST_USER_BIT + 17),
		EDGESEL7  = 1 << (BitFlags::FIRST_USER_BIT + 18),
		EDGESEL8  = 1 << (BitFlags::FIRST_USER_BIT + 19),
		EDGESEL9  = 1 << (BitFlags::FIRST_USER_BIT + 20),
		EDGESEL10 = 1 << (BitFlags::FIRST_USER_BIT + 21),
		EDGESEL11 = 1 << (BitFlags::FIRST_USER_BIT + 22) // 12 bits for selection
	};

private:
	// will use these members as isOnBorder0, setOnBorder0 and clearOnBorder0
	using BitFlags::isOnBorder;
	using BitFlags::setOnBorder;
	using BitFlags::clearOnBorder;
};

}

#include "polygon_bit_flags.cpp"

#endif // MGP_MESH_PROPERTIES_POLYGON_BIT_FLAGS_H
