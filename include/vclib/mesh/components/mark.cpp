/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "mark.h"

namespace vcl::comp {

template<typename El, bool o>
Mark<El, o>::Mark()
{
	if constexpr (!IS_VERTICAL) {
		init();
	}
}

template<typename El, bool o>
void Mark<El, o>::init()
{
	m() = 0;
}

template<typename El, bool o>
bool Mark<El, o>::isEnabled()
{
	return data.template isComponentEnabled<El>(this);
}

template<typename El, bool o>
int Mark<El, o>::mark() const
{
	return m();
}

template<typename El, bool o>
void Mark<El, o>::resetMark()
{
	m() = 0;
}

template<typename El, bool o>
template<typename E>
bool Mark<El, o>::hasSameMark(const E& e) const
{
	if constexpr (std::is_pointer<E>::value) {
		return e->mark() == m();
	}
	else {
		return e.mark() == m();
	}
}

template<typename El, bool o>
void Mark<El, o>::incrementMark()
{
	m()++;
}

template<typename El, bool o>
void Mark<El, o>::decrementMark()
{
	m()--;
}

template<typename El, bool o>
bool Mark<El, o>::isMarkEnabled() const
{
	return data.template isComponentEnabled<El>(this);
}

template<typename El, bool o>
template<typename Element>
void Mark<El, o>::importFrom(const Element& e)
{
	if constexpr (HasMark<Element>) {
		if (isMarkEnabledOn(e)) {
			m() = e.mark();
		}
	}
}

template<typename El, bool o>
int& Mark<El, o>::m()
{
	return data.template get<El>(this);
}

template<typename El, bool o>
const int& Mark<El, o>::m() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
