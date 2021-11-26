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

#include "optional_wedge_colors.h"

namespace vcl::comp {

template<int N, typename T>
OptionalWedgeColors<N, T>::OptionalWedgeColors()
{
}

template<int N, typename T>
vcl::Color& OptionalWedgeColors<N, T>::wedgeColor(uint i)
{
	assert(i < wedgeColorsNumber());
	return B::optCont().wedgeColors(thisId())[i];
}

template<int N, typename T>
const vcl::Color& OptionalWedgeColors<N, T>::wedgeColor(uint i) const
{
	assert(i < wedgeColorsNumber());
	return B::optCont().wedgeColors(thisId())[i];
}

template<int N, typename T>
vcl::Color& OptionalWedgeColors<N, T>::wedgeColorMod(int i)
{
	uint n = wedgeColorsNumber();
	return B::optCont().wedgeColors(thisId())[(i % n + n) % n];
}

template<int N, typename T>
const vcl::Color& OptionalWedgeColors<N, T>::wedgeColorMod(int i) const
{
	uint n = wedgeColorsNumber();
	return B::optCont().wedgeColors(thisId())[(i % n + n) % n];
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::setWedgeColor(const vcl::Color& t, uint i)
{
	assert(i < wedgeColorsNumber());
	B::optCont().wedgeColors(thisId())[i] = t;
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::setWedgeColors(const std::vector<vcl::Color>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& t : list) {
			setWedgeColor(t, i);
			++i;
		}
	}
	else {
		B::optCont().wedgeColors(thisId()) = list;
	}
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::WedgeColorsIterator OptionalWedgeColors<N, T>::wedgeColorBegin()
{
	return B::optCont().wedgeColors(thisId()).begin();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::WedgeColorsIterator OptionalWedgeColors<N, T>::wedgeColorEnd()
{
	return B::optCont().wedgeColors(thisId()).end();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::ConstWedgeColorsIterator
OptionalWedgeColors<N, T>::wedgeColorBegin() const
{
	return B::optCont().wedgeColors(thisId()).begin();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::ConstWedgeColorsIterator
OptionalWedgeColors<N, T>::wedgeColorEnd() const
{
	return B::optCont().wedgeColors(thisId()).end();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::WedgeColorsRangeIterator
OptionalWedgeColors<N, T>::wedgeColorIterator()
{
	return WedgeColorsRangeIterator(
		B::optCont().wedgeColors(thisId()),
		&OptionalWedgeColors::wedgeColorBegin,
		&OptionalWedgeColors::wedgeColorEnd);
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::ConstWedgeColorsRangeIterator
OptionalWedgeColors<N, T>::wedgeColorIterator() const
{
	return ConstWedgeColorsRangeIterator(
		B::optCont().wedgeColors(thisId()),
		&OptionalWedgeColors::wedgeColorBegin,
		&OptionalWedgeColors::wedgeColorEnd);
}

template<int N, typename T>
bool OptionalWedgeColors<N, T>::wedgeColorsEnabled() const
{
	return B::optCont().isWedgeColorsEnabled();
}

template<int N, typename T>
uint OptionalWedgeColors<N, T>::wedgeColorsNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().wedgeColors(thisId()).size();
	}
}

template<int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalWedgeColors<N, T>::resizeWedgeColors(uint n)
{
	B::optCont().wedgeColors(thisId()).resize(n);
}

template<int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalWedgeColors<N, T>::pushWedgeColor(const vcl::Color& t)
{
	B::optCont().wedgeColors(thisId()).push_back(t);
}

template<int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalWedgeColors<N, T>::insertWedgeColor(uint i, const vcl::Color& t)
{
	assert(i < wedgeColorsNumber());
	B::optCont().wedgeColors(thisId()).insert(B::optCont().wedgeColors(thisId()).begin() + i, t);
}

template<int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalWedgeColors<N, T>::eraseWedgeColor(uint i)
{
	assert(i < wedgeColorsNumber());
	B::optCont().wedgeColors(thisId()).erase(B::optCont().wedgeColors(thisId()).begin() + i);
}

template<int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalWedgeColors<N, T>::clearWedgeColor()
{
	B::optCont().wedgeColors(thisId()).clear();
}

} // namespace vcl::comp
