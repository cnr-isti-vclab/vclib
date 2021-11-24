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

#include "wedge_colors.h"

namespace vcl::comp {

template<int N>
WedgeColors<N>::WedgeColors() : Base()
{
}

template<int N>
vcl::Color& WedgeColors<N>::wedgeColor(unsigned int i)
{
	return Base::at(i);
}

template<int N>
const vcl::Color& WedgeColors<N>::wedgeColor(unsigned int i) const
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
void WedgeColors<N>::setWedgeColor(const vcl::Color& t, unsigned int i)
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
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::resizeWedgeColors(unsigned int n)
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
WedgeColors<N>::insertWedgeColor(unsigned int i, const vcl::Color& c)
{
	Base::insert(i, c);
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::eraseWedgeColor(unsigned int i)
{
	Base::erase(i);
}

template<int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) WedgeColors<N>::clearWedgeColor()
{
	Base::clear();
}

} // namespace vcl::comp
