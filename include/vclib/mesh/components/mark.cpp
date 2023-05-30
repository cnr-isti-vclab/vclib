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

#include "mark.h"

namespace vcl::comp {

template<typename El, bool O>
Mark<El, O>::Mark()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename El, bool O>
void Mark<El, O>::init()
{
	m() = 0;
}

template<typename El, bool O>
bool Mark<El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename El, bool O>
bool Mark<El, O>::isMarkEnabled() const
{
	return isEnabled();
}

template<typename El, bool O>
int Mark<El, O>::mark() const
{
	return m();
}

template<typename El, bool O>
void Mark<El, O>::resetMark()
{
	m() = 0;
}

template<typename El, bool O>
template<typename E>
bool Mark<El, O>::hasSameMark(const E& e) const
{
	if constexpr (std::is_pointer<E>::value) {
		return e->mark() == m();
	}
	else {
		return e.mark() == m();
	}
}

template<typename El, bool O>
void Mark<El, O>::incrementMark()
{
	m()++;
}

template<typename El, bool O>
void Mark<El, O>::decrementMark()
{
	m()--;
}

template<typename El, bool O>
template<typename Element>
void Mark<El, O>::importFrom(const Element& e)
{
	if constexpr (HasMark<Element>) {
		if (isMarkEnabledOn(e)) {
			m() = e.mark();
		}
	}
}

template<typename El, bool O>
int& Mark<El, O>::m()
{
	return Base::data(this);
}

template<typename El, bool O>
int Mark<El, O>::m() const
{
	return Base::data(this);
}

template <typename T>
bool isMarkEnabledOn(const T& element)
{
	return isComponentEnabledOn<MARK>(element);
}

} // namespace vcl::comp
