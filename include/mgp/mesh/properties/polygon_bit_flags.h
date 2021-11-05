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

protected:
	int flags2 = 0;

private:
	// will use these members as isOnBorder0, setOnBorder0 and clearOnBorder0
	using BitFlags::isOnBorder;
	using BitFlags::setOnBorder;
	using BitFlags::clearOnBorder;
};

}

#include "polygon_bit_flags.cpp"

#endif // MGP_MESH_PROPERTIES_POLYGON_BIT_FLAGS_H
