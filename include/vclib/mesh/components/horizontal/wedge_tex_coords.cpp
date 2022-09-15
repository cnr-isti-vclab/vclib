/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "wedge_tex_coords.h"

namespace vcl::comp {

template<typename Scalar, int N>
WedgeTexCoords<Scalar, N>::WedgeTexCoords() : Base()
{
}

template<typename Scalar, int N>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoord(uint i)
{
	return Base::at(i);
}

template<typename Scalar, int N>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoord(uint i) const
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
void WedgeTexCoords<Scalar, N>::setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i)
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
WedgeTexCoords<Scalar, N>::wedgeTexCoords()
{
	return Base::rangeIterator();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoords() const
{
	return Base::rangeIterator();
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::resizeWedgeTexCoords(uint n) requires (N < 0)
{
	Base::resize(n);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t) requires (N < 0)
{
	Base::pushBack(t);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::insertWedgeTexCoord(
	uint                         i,
	const vcl::TexCoord<Scalar>& t) requires(N < 0)
{
	Base::insert(i, t);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::eraseWedgeTexCoord(uint i) requires (N < 0)
{
	Base::erase(i);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::clearWedgeTexCoord() requires (N < 0)
{
	Base::clear();
}

template<typename Scalar, int N>
template<typename Element>
void WedgeTexCoords<Scalar, N>::importFrom(const Element& e)
{
	if constexpr (HasWedgeTexCoords<Element>) {
		if (isWedgeTexCoordsEnabledOn(e)) {
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::WEDGE_TEX_COORD_NUMBER) {
					importWedgeTexCoordsFrom(e);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::WEDGE_TEX_COORD_NUMBER < 0){
					if (e.vertexNumber() == N){
						importWedgeTexCoordsFrom(e);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeWedgeTexCoords(e.vertexNumber());
				importWedgeTexCoordsFrom(e);
			}
		}
	}
}

template<typename Scalar, int N>
template<typename Element>
void WedgeTexCoords<Scalar, N>::importWedgeTexCoordsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeTexCoord(i) = e.wedgeTexCoord(i).template cast<Scalar>();
	}
}

} // namespace vcl::comp
