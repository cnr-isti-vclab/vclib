/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_MUTABLE_BIT_FLAGS_H
#define MGP_MESH_COMMON_MUTABLE_BIT_FLAGS_H

#include <assert.h>

namespace mgp::common {

/**
 * @brief The MutableBitFlags represents a collection of 32 **mutable** bits that will be part of an
 * Element (e.g. Vertex, Face, ...).
 *
 * Unlike the BitFlags component, the bits of this component are *mutable*, meaning that they can be
 * modified also in a const Elements. Therefore, the constness of this component on const Elements
 * should not be trusted.
 *
 * They could be useful in algorithms which should not modify the mesh (taking as argument a
 * const Mesh), but they need for example to mark as visited some components, or just remember some
 * flags related to components during the process.
 *
 * Member function names of this class ends with a capital M, in order to be distinguished by the
 * BitFlags that are mandatory and they are tight to the constness of the Element.
 *
 * The bits have the following meaning:
 * - 0: visited: if the current Element has been visited
 * - from 1 to 31: user bits that can have custom meanings to the user
 *
 * **List of incompatible components**:
 * - common::OptionalMutableBitFlags
 */
class MutableBitFlags
{
public:
	bool isVisitedM() const;
	bool userBitFlagM(unsigned int bit) const;

	void setVisitedM() const;
	void setUserBitM(unsigned int bit) const;

	void clearAllFlagsM() const;
	void clearVisitedM() const;
	void clearUserBitM(unsigned int bit) const;

protected:
	bool flagValueM(unsigned int flag) const;
	void setFlagM(unsigned int flag) const;
	void clearFlagM(unsigned int flag) const;

	bool userBitFlagM(unsigned int bit, unsigned int firstBit) const;
	void setUserBitM(unsigned int bit, unsigned int firstBit) const;
	void clearUserBitM(unsigned int bit, unsigned int firstBit) const;

	mutable int mutableFlags = 0;
	static const unsigned int FIRST_MUTABLE_USER_BIT = 1;

	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum {
		VISITED = 1 << 0
	};
};

/**
 * Detector to check if a class has (inherits) MutableBitFlags
 */

template <typename  T>
using hasMutableBitFlagsT = std::is_base_of<MutableBitFlags, T>;

template <typename  T>
bool constexpr hasMutableBitFlags() {return hasMutableBitFlagsT<T>::value;}

} // namespace mgp::common

#include "mutable_bit_flags.cpp"

#endif // MGP_MESH_COMMON_MUTABLE_BIT_FLAGS_H
