/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "bit_flags.h"

namespace mgp {
namespace common {

inline bool BitFlags::isDeleted() const
{
	return flags & DELETED;
}

inline bool BitFlags::isSelected() const
{
	return flags & SELECTED;
}

inline bool BitFlags::isOnBorder() const
{
	return flags & BORDER;
}

inline bool BitFlags::isVisited() const
{
	return flags & VISITED;
}

inline bool BitFlags::userBitFlag(unsigned int bit)
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	return flags && bit;
}

inline void BitFlags::clearAllFlags()
{
	flags = 0;
}

inline void BitFlags::setDeleted(bool b)
{
	if (b) {
		flags |= DELETED;
	}
	else {
		flags &= ~DELETED;
	}
}

inline void BitFlags::setSelected(bool b)
{
	if (b) {
		flags |= SELECTED;
	}
	else {
		flags &= ~SELECTED;
	}
}

inline void BitFlags::setBorder(bool b)
{
	if (b) {
		flags |= BORDER;
	}
	else {
		flags &= ~BORDER;
	}
}

inline void BitFlags::setVisited(bool b)
{
	if (b) {
		flags |= VISITED;
	}
	else {
		flags &= ~VISITED;
	}
}

inline void BitFlags::setUserBit(unsigned int bit, bool b)
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	if (b) {
		flags |= bit;
	}
	else {
		flags &= ~bit;
	}
}

} // namespace common
} // namespace mgp
