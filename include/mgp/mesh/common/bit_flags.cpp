/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "bit_flags.h"

namespace mgp::common {

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

inline bool BitFlags::userBitFlag(unsigned int bit) const
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	return flags && bit;
}

/**
 * @brief BitFlags::clearAllFlags clear all the flags and sets them to false, **except the deleted
 * flag**, which needs to be manually reset.
 */
inline void BitFlags::clearAllFlags()
{
	int isD = isDeleted();
	flags    = 0;
	flags |= isD;
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

inline void BitFlags::setDeleted(bool b)
{
	if (b) {
		flags |= DELETED;
	}
	else {
		flags &= ~DELETED;
	}
}

} // namespace mgp::common
