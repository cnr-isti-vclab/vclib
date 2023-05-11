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

#include "scalar.h"

namespace vcl::comp {



template<typename T, typename El, bool O>
bool Scalar<T, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename T, typename El, bool O>
bool Scalar<T, El, O>::isScalarEnabled() const
{
	return isEnabled();
}

template<typename T, typename El, bool O>
const T& Scalar<T, El, O>::scalar() const
{
	return Base::data(this);
}

template<typename T, typename El, bool O>
T& Scalar<T, El, O>::scalar()
{
	return Base::data(this);
}

template<typename T, typename El, bool O>
template<typename Element>
void Scalar<T, El, O>::importFrom(const Element& e)
{
	if constexpr (HasScalar<Element>) {
		if (isScalarEnabledOn(e)){
			scalar() = e.scalar();
		}
	}
}

template <typename T>
bool isScalarEnabledOn(const T& element)
{
	if constexpr (HasOptionalScalar<T>) {
		return element.isScalarEnabled();
	}
	else {
		return HasScalar<T>;
	}
}

} // namespace vcl::comp
