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

#include "wedge_tex_coords.h"

namespace vcl::comp {

template<typename Scalar, int N>
WedgeTexCoords<Scalar, N>::WedgeTexCoords() : Base()
{
}

template<typename Scalar, int N>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoord(unsigned int i)
{
	return Base::at(i);
}

template<typename Scalar, int N>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoord(unsigned int i) const
{
	return Base::at(i);
}

template<typename Scalar, int N>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoordMod(int i)
{
	return Base::atMod(i);
}

template<typename Scalar, int N>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoordMod(int i) const
{
	return Base::atMod(i);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, unsigned int i)
{
	Base::set(t, i);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar> >& list)
{
	Base::set(list);
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordBegin()
{
	return Base::begin();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordEnd()
{
	return Base::end();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordBegin() const
{
	return Base::begin();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordEnd() const
{
	return Base::end();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::WedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordIterator()
{
	return Base::rangeIterator();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordIterator() const
{
	return Base::rangeIterator();
}

template<typename Scalar, int N>
template<int U>
internal::ReturnIfIsVector<U, void> WedgeTexCoords<Scalar, N>::resizeWedgeTexCoords(unsigned int n)
{
	Base::resize(n);
}

template<typename Scalar, int N>
template<int U>
internal::ReturnIfIsVector<U, void> WedgeTexCoords<Scalar, N>::pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t)
{
	Base::pushBack(t);
}

template<typename Scalar, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
WedgeTexCoords<Scalar, N>::insertWedgeTexCoord(unsigned int i, const vcl::TexCoord<Scalar>& t)
{
	Base::insert(i, t);
}

template<typename Scalar, int N>
template<int U>
internal::ReturnIfIsVector<U, void> WedgeTexCoords<Scalar, N>::eraseWedgeTexCoord(unsigned int i)
{
	Base::erase(i);
}

template<typename Scalar, int N>
template<int U>
internal::ReturnIfIsVector<U, void> WedgeTexCoords<Scalar, N>::clearWedgeTexCoord()
{
	Base::clear();
}

} // namespace vcl::comp
