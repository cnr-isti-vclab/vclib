/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_BIT_FLAGS_H
#define MGP_MESH_COMMON_BIT_FLAGS_H

#include <assert.h>

#include "info.h"

namespace mgp {
namespace common {

/**
 * @brief The BitFlags class represents a collection of 32 bits that will be part of an Element
 * (Vertex, Face, ...).
 *
 * This component is mandatory into every Element of the mesh.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Element has been deleted
 * - 1: selected: if the current Element has been selected
 * - 2: border: if the current Element is on border
 * - 3: visited: if the current Element has been visited
 * - from 4 to 31: user bits that can have custom meanings to the user
 */
class BitFlags : public virtual Info
{
public:
	bool isDeleted() const;
	bool isSelected() const;
	bool isOnBorder() const;
	bool isVisited() const;
	bool userBitFlag(unsigned int bit);

	void clearAllFlags();
	void setSelected(bool b = true);
	void setBorder(bool b = true);
	void setVisited(bool b = true);
	void setUserBit(unsigned int bit, bool b = true);

protected:
	void setDeleted(bool b = true);

private:
	static const unsigned int FIST_USER_BIT = 4;
	enum {
		DELETED  = 1 << 0, // first bit
		SELECTED = 1 << 1, // second
		BORDER   = 1 << 2, // third
		VISITED  = 1 << 3  // fourth
	};
	int flags = 0;
};

} // namespace common
} // namespace mgp

#include "bit_flags.cpp"

#endif // MGP_MESH_COMMON_BIT_FLAGS_H
