/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_mutable_bit_flags.h"

namespace mgp::common {

/**
 * @brief OptionalMutableBitFlags::isVisitedM returns whether the Element is marked as visited.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @return true if the Element is visited, false otherwise.
 */
template<typename T>
bool OptionalMutableBitFlags<T>::isVisitedM() const
{
	return flagValueM(VISITED);
}

/**
 * @brief OptionalMutableBitFlags::userBitFlagM returns the value of the bit given in input. The bit
 * is checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
template<typename T>
bool OptionalMutableBitFlags<T>::userBitFlagM(unsigned int bit) const
{
	return userBitFlagM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief OptionalMutableBitFlags::setVisitedM marks as visited the Element.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
template<typename T>
void OptionalMutableBitFlags<T>::setVisitedM() const
{
	setFlagM(VISITED);
}

/**
 * @brief OptionalMutableBitFlags::setUserBitM sets to true the value of the bit given in input. The
 * bit is checked to be greater or equal than the first user bit available in this class (1) and
 * less than 32.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param bit: the position of the bit that will be set.
 */
template<typename T>
void OptionalMutableBitFlags<T>::setUserBitM(unsigned int bit) const
{
	setUserBitM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief OptionalMutableBitFlags::clearAllFlagsM clear all the flags and sets them to false.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 */
template<typename T>
void OptionalMutableBitFlags<T>::clearAllFlagsM() const
{
	int& flags = B::containerPointer->flags(B::id());
	flags      = 0;
}

/**
 * @brief OptionalMutableBitFlags::clearVisitedM marks as non-visited the Element.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 */
template<typename T>
void OptionalMutableBitFlags<T>::clearVisitedM() const
{
	clearFlagM(VISITED);
}

/**
 * @brief OptionalMutableBitFlags::clearUserBitM sets to false the value of the bit given in input. The bit
 * is checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param bit: the position of the bit that will be reset.
 */
template<typename T>
void OptionalMutableBitFlags<T>::clearUserBitM(unsigned int bit) const
{
	clearUserBitM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief OptionalMutableBitFlags::flagValueM  returns the value of the given flag (value of the
 * enum).
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param flag: value of the flag to return.
 * @return whether the flag is true or false.
 */
template<typename T>
bool OptionalMutableBitFlags<T>::flagValueM(unsigned int flag) const
{
	const int& flags = B::containerPointer->flags(B::id());
	return flags & flag;
}

/**
 * @brief OptionalMutableBitFlags::setFlagM sets to true the value of the given flag (value of the
 * enum).
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param flag: value of the flag to set.
 */
template<typename T>
void OptionalMutableBitFlags<T>::setFlagM(unsigned int flag) const
{
	int& flags = B::containerPointer->flags(B::id());
	flags |= flag;
}

/**
 * @brief OptionalMutableBitFlags::clearFlagM sets to false the value of the given flag (value of
 * the enum).
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param flag: value of the flag to reset.
 */
template<typename T>
void OptionalMutableBitFlags<T>::clearFlagM(unsigned int flag) const
{
	int& flags = B::containerPointer->flags(B::id());
	flags &= ~flag;
}

/**
 * @brief OptionalMutableBitFlags::userBitFlagM returns the value of the bit given in input. The bit
 * is checked to be greateror equal  than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 * @return the value of the bit
 */
template<typename T>
bool OptionalMutableBitFlags<T>::userBitFlagM(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	return flagValueM(flag);
}

/**
 * @brief OptionalMutableBitFlags::setUserBitM sets to true the value of the bit given in input. The
 * bit is checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
template<typename T>
void OptionalMutableBitFlags<T>::setUserBitM(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	setFlagM(flag);
}

/**
 * @brief OptionalMutableBitFlags::clearUserBitM sets to false the value of the bit given in input.
 * The bit is checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 * @warning to use this member function, you have to enable first the MutableFlags in the container.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 * @param bit
 * @param firstBit
 */
template<typename T>
void OptionalMutableBitFlags<T>::clearUserBitM(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	clearFlagM(flag);
}

} // namespace mgp::common
