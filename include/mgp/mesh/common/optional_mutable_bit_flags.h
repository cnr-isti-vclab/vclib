/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_BIT_FLAGS_H
#define MGP_MESH_COMMON_OPTIONAL_BIT_FLAGS_H

#include <assert.h>

#include "optional_info.h"

namespace mgp::common {

/**
 * @brief The OptionalMutableBitFlags represents a collection of 32 **mutable** bits that will be
 * part of an Element (e.g. Vertex, Face, ...).
 *
 * This is an Optional Component, meaning that can be enabled and/or disabled at runtime. By
 * default, these components are disabled.
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
 */
template<typename T>
class OptionalMutableBitFlags : public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;

public:
	bool isVisitedM() const;
	bool userBitFlagM(unsigned int bit) const;

	void clearAllFlagsM() const;
	void setVisitedM(bool b = true) const;
	void setUserBitM(unsigned int bit, bool b = true) const;

	void __optional_mutable_bit_flags__() {
	} // dummy member used just for detection of an OpionalBitFlags object

private:
	static const unsigned int FIST_USER_BIT = 1;
	enum {
		VISITED  = 1 << 0
	};
};

/**
 * Detector to check if a class has (inherits) OptionalBitFlags
 */

template<typename T>
using hasOptionalMutableBitFlags_t = decltype(std::declval<T&>().__optional_mutable_bit_flags__());

template<typename T>
using hasOptionalMutableBitFlags = typename detector<hasOptionalMutableBitFlags_t, void, T>::type;

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = typename std::enable_if<hasOptionalMutableBitFlags<U>::value, T>::type;

} // namespace mgp::common

#include "optional_mutable_bit_flags.cpp"

#endif // MGP_MESH_COMMON_OPTIONAL_BIT_FLAGS_H
