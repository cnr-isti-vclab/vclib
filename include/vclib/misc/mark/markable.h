/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MISC_MARK_MARKABLE_H
#define VCL_MISC_MARK_MARKABLE_H

#include <vclib/misc/types.h>

namespace vcl {

/**
 * @brief The Markable class is a simple wrapper that makes a Type "Markable", that is
 * just adding an unsigned int that can be accessed using the mark() member function.
 *
 * To access to the wrapped value, you can use the `get()` member function.
 */

// we use wrapping instead of inheritance since it is forbidden to inherit from non-class types
// (e.g. pointers, integers, floats...).
template<typename Type>
class Markable
{
public:
	Markable(const Type& t) : t(t) {}

	uint&       mark() const { return m; }
	Type&       get() { return t; }
	const Type& get() const { return t; }

private:
	mutable uint m = 0;
	Type t;
};

} // namespace vcl

#endif // VCL_MISC_MARK_MARKABLE_H
