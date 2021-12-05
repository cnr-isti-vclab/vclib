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

#include "wedge_colors.h"

namespace vcl::comp {

template<int N>
WedgeColors<N>::WedgeColors() : Base()
{
}

template<int N>
vcl::Color& WedgeColors<N>::wedgeColor(uint i)
{
	return Base::at(i);
}

template<int N>
const vcl::Color& WedgeColors<N>::wedgeColor(uint i) const
{
	return Base::at(i);
}

template<int N>
vcl::Color& WedgeColors<N>::wedgeColorMod(int i)
{
	return Base::atMod(i);
}

template<int N>
const vcl::Color& WedgeColors<N>::wedgeColorMod(int i) const
{
	return Base::atMod(i);
}

template<int N>
void WedgeColors<N>::setWedgeColor(const vcl::Color& t, uint i)
{
	Base::set(t, i);
}

template<int N>
void WedgeColors<N>::setWedgeColors(const std::vector<vcl::Color>& list)
{
	Base::set(list);
}

template<int N>
typename WedgeColors<N>::WedgeColorsIterator WedgeColors<N>::wedgeColorBegin()
{
	return Base::begin();
}

template<int N>
typename WedgeColors<N>::WedgeColorsIterator WedgeColors<N>::wedgeColorEnd()
{
	return Base::end();
}

template<int N>
typename WedgeColors<N>::ConstWedgeColorsIterator WedgeColors<N>::wedgeColorBegin() const
{
	return Base::begin();
}

template<int N>
typename WedgeColors<N>::ConstWedgeColorsIterator WedgeColors<N>::wedgeColorEnd() const
{
	return Base::end();
}

template<int N>
typename WedgeColors<N>::WedgeColorsRangeIterator WedgeColors<N>::wedgeColorIterator()
{
	return Base::rangeIterator();
}

template<int N>
typename WedgeColors<N>::ConstWedgeColorsRangeIterator WedgeColors<N>::wedgeColorIterator() const
{
	return Base::rangeIterator();
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::resizeWedgeColors(uint n)
{
	Base::resize(n);
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::pushWedgeColor(const vcl::Color& c)
{
	Base::pushBack(c);
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
WedgeColors<N>::insertWedgeColor(uint i, const vcl::Color& c)
{
	Base::insert(i, c);
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::eraseWedgeColor(uint i)
{
	Base::erase(i);
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::clearWedgeColor()
{
	Base::clear();
}

template<int N>
template<typename Element>
void WedgeColors<N>::importFrom(const Element& e)
{
	if constexpr (hasWedgeColors<Element>()) {
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
				resizeWedgeColors(e.vertexNumber());
				importWedgeColorsFrom(e);
			}
		}
	}
}

template<int N>
template<typename Element>
void WedgeColors<N>::importWedgeColorsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeColor(i) = e.wedgeColor(i);
	}
}

} // namespace vcl::comp
