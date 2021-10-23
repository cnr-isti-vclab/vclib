/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mutable_bit_flags.h"

namespace mgp::common {

bool MutableBitFlags::isSelectedM() const
{
	return flags & SELECTED;
}

bool MutableBitFlags::isOnBorderM() const
{
	return flags & BORDER;
}

bool MutableBitFlags::isVisitedM() const
{
	return flags & VISITED;
}

bool MutableBitFlags::userBitFlagM(unsigned int bit) const
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	return flags && bit;
}

void MutableBitFlags::clearAllFlagsM() const
{
	flags = 0;
}

void MutableBitFlags::setSelectedM(bool b) const
{
	if (b) {
		flags |= SELECTED;
	}
	else {
		flags &= ~SELECTED;
	}
}

void MutableBitFlags::setBorderM(bool b) const
{
	if (b) {
		flags |= BORDER;
	}
	else {
		flags &= ~BORDER;
	}
}

void MutableBitFlags::setVisitedM(bool b) const
{
	if (b) {
		flags |= VISITED;
	}
	else {
		flags &= ~VISITED;
	}
}

void MutableBitFlags::setUserBitM(unsigned int bit, bool b) const
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

} // namespace mgp::common
