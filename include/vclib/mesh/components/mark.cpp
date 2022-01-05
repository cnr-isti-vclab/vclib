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

#include "mark.h"

namespace vcl::comp {

inline int Mark::mark() const
{
	return m;
}

inline void Mark::resetMark()
{
	m = 0;
}

template<typename E>
bool Mark::hasSameMark(const E& e) const
{
	if constexpr (std::is_pointer<E>::value) {
		return e->mark() == m;
	}
	else {
		return e.mark() == m;
	}
}

inline void Mark::incrementMark()
{
	m++;
}

inline void Mark::decrementMark()
{
	m--;
}

template<typename Element>
void Mark::importFrom(const Element& e)
{
	if constexpr (hasMark<Element>()) {
		if (isMarkEnabledOn(e)) {
			m = e.mark();
		}
	}
}

} // namespace vcl::comp
