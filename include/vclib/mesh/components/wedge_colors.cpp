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

#include "wedge_colors.h"

#include <vclib/concepts/mesh/components/color.h>

namespace vcl::comp {

template<int N, typename El, bool O>
bool WedgeColors<N, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<int N, typename El, bool O>
vcl::Color& WedgeColors<N, El, O>::wedgeColor(uint i)
{
	return colors().at(i);
}

template<int N, typename El, bool O>
const vcl::Color& WedgeColors<N, El, O>::wedgeColor(uint i) const
{
	return colors().at(i);
}

template<int N, typename El, bool O>
vcl::Color& WedgeColors<N, El, O>::wedgeColorMod(int i)
{
	return colors().atMod(i);
}

template<int N, typename El, bool O>
const vcl::Color& WedgeColors<N, El, O>::wedgeColorMod(int i) const
{
	return colors().atMod(i);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::setWedgeColor(const vcl::Color& t, uint i)
{
	colors().set(t, i);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::setWedgeColors(const std::vector<vcl::Color>& list)
{
	colors().set(list);
}

template<int N, typename El, bool O>
typename WedgeColors<N, El, O>::WedgeColorsIterator WedgeColors<N, El, O>::wedgeColorBegin()
{
	return colors().begin();
}

template<int N, typename El, bool O>
typename WedgeColors<N, El, O>::WedgeColorsIterator WedgeColors<N, El, O>::wedgeColorEnd()
{
	return colors().end();
}

template<int N, typename El, bool O>
typename WedgeColors<N, El, O>::ConstWedgeColorsIterator WedgeColors<N, El, O>::wedgeColorBegin() const
{
	return colors().begin();
}

template<int N, typename El, bool O>
typename WedgeColors<N, El, O>::ConstWedgeColorsIterator WedgeColors<N, El, O>::wedgeColorEnd() const
{
	return colors().end();
}

template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColors()
{
	return View(wedgeColorBegin(), wedgeColorEnd());
}

template<int N, typename El, bool O>
auto WedgeColors<N, El, O>::wedgeColors() const
{
	return View(wedgeColorBegin(), wedgeColorEnd());
}

template<int N, typename El, bool O>
template<typename Element>
void WedgeColors<N, El, O>::importFrom(const Element& e)
{
	if constexpr (HasWedgeColors<Element>) {
		if (isWedgeColorsEnabledOn(e)) {
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::WEDGE_COLOR_NUMBER) {
					importWedgeColorsFrom(e);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::WEDGE_COLOR_NUMBER < 0){
					if (e.vertexNumber() == N){
						importWedgeColorsFrom(e);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resize(e.vertexNumber());
				importWedgeColorsFrom(e);
			}
		}
	}
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::resize(uint n) requires (N < 0)
{
	colors().resize(n);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::pushBack(const vcl::Color& c) requires (N < 0)
{
	colors().pushBack(c);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::insert(uint i, const vcl::Color& c) requires (N < 0)
{
	colors().insert(i, c);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::erase(uint i) requires (N < 0)
{
	colors().erase(i);
}

template<int N, typename El, bool O>
void WedgeColors<N, El, O>::clear() requires (N < 0)
{
	colors().clear();
}

template<int N, typename El, bool O>
template<typename Element>
void WedgeColors<N, El, O>::importWedgeColorsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeColor(i) = e.wedgeColor(i);
	}
}

template<int N, typename El, bool O>
Vector<vcl::Color, N>& WedgeColors<N, El, O>::colors()
{
	return Base::container(this);
}

template<int N, typename El, bool O>
const Vector<vcl::Color, N>& WedgeColors<N, El, O>::colors() const
{
	return Base::container(this);
}

template <typename T>
bool isWedgeColorsEnabledOn(const T& element)
{
	return isComponentEnabledOn<WEDGE_COLORS>(element);
}

} // namespace vcl::comp
