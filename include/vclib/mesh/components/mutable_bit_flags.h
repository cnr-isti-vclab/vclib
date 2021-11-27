/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_MUTABLE_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_MUTABLE_BIT_FLAGS_H

#include <assert.h>
#include <type_traits>

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

/**
 * @brief The MutableBitFlags represents a collection of 32 **mutable** bits that will be part of an
 * Element (e.g. Vertex, Face, ...).
 *
 * Unlike the BitFlags component, the bits of this component are *mutable*, meaning that they can be
 * modified also in a const Elements. Therefore, the constness of this component on const Elements
 * should not be trusted.
 *
 * They could be useful in algorithms which should not modify the mesh (taking as argument a
 * const Mesh), but they need for example to mark as visited some element, or just remember some
 * flags related to the element during the process.
 *
 * Member function names of this class ends with a capital M, in order to be distinguished by the
 * BitFlags that are mandatory and they are tight to the constness of the Element.
 *
 * The bits have the following meaning:
 * - 0: visited: if the current Element has been visited
 * - from 1 to 31: user bits that can have custom meanings to the user
 *
 * **List of incompatible comp**:
 * - common::OptionalMutableBitFlags
 */
class MutableBitFlags
{
public:
	bool isVisitedM() const;
	bool userBitFlagM(uint bit) const;

	void setVisitedM() const;
	void setUserBitM(uint bit) const;

	void clearAllFlagsM() const;
	void clearVisitedM() const;
	void clearUserBitM(uint bit) const;

protected:
	bool flagValueM(uint flag) const;
	void setFlagM(uint flag) const;
	void clearFlagM(uint flag) const;

	bool userBitFlagM(uint bit, uint firstBit) const;
	void setUserBitM(uint bit, uint firstBit) const;
	void clearUserBitM(uint bit, uint firstBit) const;

	mutable int mutableFlags = 0;

	static const uint FIRST_MUTABLE_USER_BIT = 1;

	// values of the flags, used for flagValue, setFlag and clearFlag member functions
	enum { VISITED = 1 << 0 };
};

/**
 * Detector to check if a class has (inherits) MutableBitFlags
 */

template<typename T>
using hasMutableBitFlagsT = std::is_base_of<MutableBitFlags, T>;

template<typename T>
bool constexpr hasMutableBitFlags()
{
	return hasMutableBitFlagsT<T>::value;
}

} // namespace vcl::comp

#include "mutable_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_MUTABLE_BIT_FLAGS_H
