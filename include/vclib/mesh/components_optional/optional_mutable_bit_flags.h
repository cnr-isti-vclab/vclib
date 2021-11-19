/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_OPTIONAL_BIT_FLAGS_H

#include <assert.h>

#include "optional_info.h"

namespace vcl::comp {

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
 *
 * **List of incompatible components**:
 * - common::MutableBitFlags
 */
template<typename T>
class OptionalMutableBitFlags : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

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

	static const unsigned int FIRST_MUTABLE_USER_BIT = 1;

	enum { VISITED = 1 << 0 };

private:
	unsigned int thisId() const { return B::index((T*)this); }
};

/**
 * Detector to check if a class has (inherits) OptionalMutableBitFlags
 */

template<typename T>
using hasOptionalMutableBitFlagsT = std::is_base_of<OptionalMutableBitFlags<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags =
	typename std::enable_if<hasOptionalMutableBitFlagsT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalMutableBitFlags()
{
	return hasOptionalMutableBitFlagsT<T>::value;
}

} // namespace vcl::comp

#include "optional_mutable_bit_flags.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_BIT_FLAGS_H
