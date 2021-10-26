/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mutable_bit_flags.h"

namespace mgp::common {

/**
 * @brief MutableBitFlags::isVisitedM returns whether the Element is marked as visited.
 *
 * @return true if the Element is visited, false otherwise.
 */
inline bool MutableBitFlags::isVisitedM() const
{
	return flagValueM(VISITED);
}

/**
 * @brief MutableBitFlags::userBitFlagM returns the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool MutableBitFlags::userBitFlagM(unsigned int bit) const
{
	return userBitFlagM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief MutableBitFlags::setVisitedM marks as visited the Element.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
inline void MutableBitFlags::setVisitedM() const
{
	setFlagM(VISITED);
}

/**
 * @brief MutableBitFlags::setUserBitM sets to true the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: the position of the bit that will be set.
 */
inline void MutableBitFlags::setUserBitM(unsigned int bit) const
{
	setUserBitM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief MutableBitFlags::clearAllFlagsM clear all the flags and sets them to false.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
inline void MutableBitFlags::clearAllFlagsM() const
{
	mutableFlags = 0;
}

/**
 * @brief MutableBitFlags::clearVisitedM marks as non-visited the Element.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
inline void MutableBitFlags::clearVisitedM() const
{
	clearFlagM(VISITED);
}

/**
 * @brief MutableBitFlags::clearUserBitM sets to false the value of the bit given in input. The bit
 * is checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: the position of the bit that will be reset.
 */
inline void MutableBitFlags::clearUserBitM(unsigned int bit) const
{
	clearUserBitM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief MutableBitFlags::flagValueM returns the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to return.
 * @return whether the flag is true or false.
 */
inline bool MutableBitFlags::flagValueM(unsigned int flag) const
{
	return mutableFlags & flag;
}

/**
 * @brief MutableBitFlags::setFlagM sets to true the value of the given flag (value of the enum).
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param flag: value of the flag to set.
 */
inline void MutableBitFlags::setFlagM(unsigned int flag) const
{
	mutableFlags |= flag;
}

/**
 * @brief MutableBitFlags::clearFlagM sets to false the value of the given flag (value of the enum).
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param flag: value of the flag to reset.
 */
inline void MutableBitFlags::clearFlagM(unsigned int flag) const
{
	mutableFlags &= ~flag;
}

/**
 * @brief MutableBitFlags::userBitFlagM returns the value of the bit given in input. The bit is
 * checked to be greateror equal  than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 * @return the value of the bit
 */
inline bool MutableBitFlags::userBitFlagM(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	return flagValueM(flag);
}

/**
 * @brief MutableBitFlags::setUserBitM sets to true the value of the bit given in input. The bit is
 * checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void MutableBitFlags::setUserBitM(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	setFlagM(flag);
}

/**
 * @brief MutableBitFlags::clearUserBitM sets to false the value of the bit given in input. The bit is
 * checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 * @param bit
 * @param firstBit
 */
inline void MutableBitFlags::clearUserBitM(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	clearFlagM(flag);
}

} // namespace mgp::common
