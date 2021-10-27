/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_BIT_FLAGS_H
#define MGP_MESH_COMMON_BIT_FLAGS_H

#include <assert.h>

#include <mgp/misc/type_traits.h>

#include "info.h"

namespace mgp::common {

/**
 * @brief The BitFlags class represents a collection of 32 bits that will be part of an Element
 * (e.g. Vertex, Face, ...).
 *
 * This component is mandatory into every Element of the mesh.
 *
 * The bits have the following meaning:
 * - 0: deleted: if the current Element has been deleted
 * - 1: selected: if the current Element has been selected
 * - 2: border: if the current Element is on border
 * - from 3 to 31: user bits that can have custom meanings to the user
 */
class BitFlags : public virtual Info
{
public:
	bool isDeleted() const;
	bool isSelected() const;
	bool isOnBorder() const;
	bool userBitFlag(unsigned int bit) const;

	void setSelected();
	void setOnBorder();
	void setUserBit(unsigned int bit);

	void clearAllFlags();
	void clearSelected();
	void clearOnBorder();
	void clearUserBit(unsigned int bit);

protected:
	void setDeleted();
	void clearDeleted();

	bool flagValue(unsigned int flag) const;
	void setFlag(unsigned int flag);
	void clearFlag(unsigned int flag);

	bool userBitFlag(unsigned int bit, unsigned int firstBit) const;
	void setUserBit(unsigned int bit, unsigned int firstBit);
	void clearUserBit(unsigned int bit, unsigned int firstBit);

	int flags = 0;
	static const unsigned int FIRST_USER_BIT = 3;

	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		DELETED  = 1 << 0, // bit 0
		SELECTED = 1 << 1, // bit 1
		BORDER   = 1 << 2  // bit 2
	};
};

/**
 * Detector to check if a class has (inherits) BitFlags
 */

template<typename T>
using hasBitFlags_t = decltype(std::declval<T&>().isDeleted());

template<typename T>
using hasBitFlags = typename detector<hasBitFlags_t, void, T>::type;

template <typename T>
constexpr bool hasBitFlagsComponent() { return hasBitFlags<T>::value;}

} // namespace mgp::common

#include "bit_flags.cpp"

#endif // MGP_MESH_COMMON_BIT_FLAGS_H
