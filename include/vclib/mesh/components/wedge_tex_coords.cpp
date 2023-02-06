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

#include "wedge_tex_coords.h"

namespace vcl::comp {

template<typename Scalar, int N>
WedgeTexCoords<Scalar, N>::WedgeTexCoords()
{
}

template<typename Scalar, int N>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoord(uint i)
{
	return texCoords.at(i);
}

template<typename Scalar, int N>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoord(uint i) const
{
	return texCoords.at(i);
}

template<typename Scalar, int N>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoordMod(int i)
{
	return texCoords.atMod(i);
}

template<typename Scalar, int N>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N>::wedgeTexCoordMod(int i) const
{
	return texCoords.atMod(i);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i)
{
	texCoords.set(t, i);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar> >& list)
{
	texCoords.set(list);
}

template<typename Scalar, int N>
short& WedgeTexCoords<Scalar, N>::textureIndex()
{
	return texIndex;
}

template<typename Scalar, int N>
const short& WedgeTexCoords<Scalar, N>::textureIndex() const
{
	return texIndex;
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordBegin()
{
	return texCoords.begin();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordEnd()
{
	return texCoords.end();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordBegin() const
{
	return texCoords.begin();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoordEnd() const
{
	return texCoords.end();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::WedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoords()
{
	return texCoords.rangeIterator();
}

template<typename Scalar, int N>
typename WedgeTexCoords<Scalar, N>::ConstWedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N>::wedgeTexCoords() const
{
	return texCoords.rangeIterator();
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::resizeWedgeTexCoords(uint n) requires (N < 0)
{
	texCoords.resize(n);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t) requires (N < 0)
{
	texCoords.pushBack(t);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::insertWedgeTexCoord(
	uint                         i,
	const vcl::TexCoord<Scalar>& t) requires(N < 0)
{
	texCoords.insert(i, t);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::eraseWedgeTexCoord(uint i) requires (N < 0)
{
	texCoords.erase(i);
}

template<typename Scalar, int N>
void WedgeTexCoords<Scalar, N>::clearWedgeTexCoord() requires (N < 0)
{
	texCoords.clear();
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
	texIndex = e.textureIndex();
}

} // namespace vcl::comp
