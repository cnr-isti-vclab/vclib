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

#include "quality.h"

namespace vcl::comp {

template<typename T, typename El, bool O>
bool Quality<T, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename T, typename El, bool O>
bool Quality<T, El, O>::isQualityEnabled() const
{
	return isEnabled();
}

template<typename T, typename El, bool O>
const T& Quality<T, El, O>::quality() const
{
	return Base::data(this);
}

template<typename T, typename El, bool O>
T& Quality<T, El, O>::quality()
{
	return Base::data(this);
}

template<typename T, typename El, bool O>
template<typename Element>
void Quality<T, El, O>::importFrom(const Element& e)
{
	if constexpr (HasQuality<Element>) {
		if (isQualityEnabledOn(e)){
			quality() = e.quality();
		}
	}
}

template <typename T>
bool isQualityEnabledOn(const T& element)
{
	if constexpr (HasOptionalQuality<T>) {
		return element.isQualityEnabled();
	}
	else {
		return HasQuality<T>;
	}
}

} // namespace vcl::comp
