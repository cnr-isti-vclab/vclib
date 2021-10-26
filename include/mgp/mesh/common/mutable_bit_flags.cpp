/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mutable_bit_flags.h"

namespace mgp::common {

bool MutableBitFlags::isVisitedM() const
{
	return mutableFlags & VISITED;
}

bool MutableBitFlags::userBitFlagM(unsigned int bit) const
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	return mutableFlags && bit;
}

void MutableBitFlags::clearAllFlagsM() const
{
	mutableFlags = 0;
}

void MutableBitFlags::setVisitedM(bool b) const
{
	if (b) {
		mutableFlags |= VISITED;
	}
	else {
		mutableFlags &= ~VISITED;
	}
}

void MutableBitFlags::setUserBitM(unsigned int bit, bool b) const
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	if (b) {
		mutableFlags |= bit;
	}
	else {
		mutableFlags &= ~bit;
	}
}

} // namespace mgp::common
