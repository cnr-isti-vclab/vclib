/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "bit_flags.h"

namespace mgp::components {

/**
 * @brief BitFlags::isDeleted returns whether the Element is marked as deleted.
 *
 * @return true if the Element is deleted, false otherwise.
 */
inline bool BitFlags::isDeleted() const
{
	return flagValue(DELETED);
}

/**
 * @brief BitFlags::isSelected returns whether the Element is marked as selected.
 *
 * @return true if the Element is seleced, false otherwise.
 */
inline bool BitFlags::isSelected() const
{
	return flagValue(SELECTED);
}

/**
 * @brief BitFlags::isOnBorder returns whether the Element is marked to be on border.
 *
 * @return true if the Element is marked to be on border, false otherwise.
 */
inline bool BitFlags::isOnBorder() const
{
	return flagValue(BORDER);
}

/**
 * @brief BitFlags::userBitFlag returns the value of the bit given in input. The bit is checked to
 * be greater or equal than the first user bit available in this class (3) and less than 32.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool BitFlags::userBitFlag(unsigned int bit) const
{
	return userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief BitFlags::setSelected marks as selected the Element.
 */
inline void BitFlags::setSelected()
{
	setFlag(SELECTED);
}

/**
 * @brief BitFlags::setOnBorder marks as on-border the Element.
 */
inline void BitFlags::setOnBorder()
{
	setFlag(BORDER);
}

/**
 * @brief BitFlags::setUserBit sets to true the value of the bit given in input. The bit is checked
 * to be greater or equal than the first user bit available in this class (3) and less than 32.
 *
 * @param bit: the position of the bit that will be set.
 */
inline void BitFlags::setUserBit(unsigned int bit)
{
	setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief BitFlags::clearAllFlags clear all the flags and sets them to false, **except the deleted
 * flag**, which needs to be manually reset.
 */
inline void BitFlags::clearAllFlags()
{
	bool isD = isDeleted();
	flags    = 0;
	if (isD)
		setFlag(DELETED);
}

/**
 * @brief BitFlags::clearSelected marks as non-selected the Element.
 */
inline void BitFlags::clearSelected()
{
	clearFlag(SELECTED);
}

/**
 * @brief BitFlags::clearOnBorder marks as non-on-border the Element.
 */
inline void BitFlags::clearOnBorder()
{
	clearFlag(BORDER);
}

/**
 * @brief BitFlags::clearUserBit sets to false the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (3) and less
 * than 32.
 *
 * @param bit: the position of the bit that will be reset.
 */
inline void BitFlags::clearUserBit(unsigned int bit)
{
	clearUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief BitFlags::setDeleted marks as deleted the Element.
 */
inline void BitFlags::setDeleted()
{
	setFlag(DELETED);
}

/**
 * @brief BitFlags::clearDeleted marks as non-deleted the Element.
 */
inline void BitFlags::clearDeleted()
{
	clearFlag(DELETED);
}

/**
 * @brief BitFlags::flagValue returns the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to return.
 * @return whether the flag is true or false.
 */
inline bool BitFlags::flagValue(unsigned int flag) const
{
	return flags & flag;
}

/**
 * @brief BitFlags::setFlag sets to true the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to set.
 */
inline void BitFlags::setFlag(unsigned int flag)
{
	flags |= flag;
}

/**
 * @brief BitFlags::clearFlag sets to false the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to reset.
 */
inline void BitFlags::clearFlag(unsigned int flag)
{
	flags &= ~flag;
}

/**
 * @brief BitFlags::userBitFlag returns the value of the bit given in input. The bit is checked
 * to be greateror equal  than the firstBit and less than 32.
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
inline bool BitFlags::userBitFlag(unsigned int bit, unsigned int firstBit) const
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	return flagValue(flag);
}

/**
 * @brief BitFlags::setUserBit sets to true the value of the bit given in input. The bit is checked
 * to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void BitFlags::setUserBit(unsigned int bit, unsigned int firstBit)
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	setFlag(flag);
}

/**
 * @brief BitFlags::clearUserBit sets to false the value of the bit given in input. The bit is
 * checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void BitFlags::clearUserBit(unsigned int bit, unsigned int firstBit)
{
	assert(bit >= firstBit && bit < 32);
	unsigned int flag = 1 << bit;
	clearFlag(flag);
}

} // namespace mgp::components
