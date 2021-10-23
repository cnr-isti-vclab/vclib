/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_mutable_bit_flags.h"

namespace mgp::common {

template<typename T>
bool OptionalMutableBitFlags<T>::isSelectedM() const
{
	int& flags = B::containerPointer->flags(B::id());
	return flags & SELECTED;
}

template<typename T>
bool OptionalMutableBitFlags<T>::isOnBorderM() const
{
	int& flags = B::containerPointer->flags(B::id());
	return flags & BORDER;
}

template<typename T>
bool OptionalMutableBitFlags<T>::isVisitedM() const
{
	int& flags = B::containerPointer->flags(B::id());
	return flags & VISITED;
}

template<typename T>
bool OptionalMutableBitFlags<T>::userBitFlagM(unsigned int bit) const
{
	int& flags = B::containerPointer->flags(B::id());
	assert(bit >= FIST_USER_BIT && bit < 32);
	bit = 1 << bit;
	return flags && bit;
}

template<typename T>
void OptionalMutableBitFlags<T>::clearAllFlagsM() const
{
	int& flags = B::containerPointer->flags(B::id());
	flags      = 0;
}

template<typename T>
void OptionalMutableBitFlags<T>::setSelectedM(bool b) const
{
	int& flags = B::containerPointer->flags(B::id());
	if (b) {
		flags |= SELECTED;
	}
	else {
		flags &= ~SELECTED;
	}
}

template<typename T>
void OptionalMutableBitFlags<T>::setBorderM(bool b) const
{
	int& flags = B::containerPointer->flags(B::id());
	if (b) {
		flags |= BORDER;
	}
	else {
		flags &= ~BORDER;
	}
}

template<typename T>
void OptionalMutableBitFlags<T>::setVisitedM(bool b) const
{
	int& flags = B::containerPointer->flags(B::id());
	if (b) {
		flags |= VISITED;
	}
	else {
		flags &= ~VISITED;
	}
}

template<typename T>
void OptionalMutableBitFlags<T>::setUserBitM(unsigned int bit, bool b) const
{
	assert(bit >= FIST_USER_BIT && bit < 32);
	int& flags = B::containerPointer->flags(B::id());
	bit        = 1 << bit;
	if (b) {
		flags |= bit;
	}
	else {
		flags &= ~bit;
	}
}

} // namespace mgp::common
