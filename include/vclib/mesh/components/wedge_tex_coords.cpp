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

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::init()
{
	texIndex() = 0;
}

template<typename Scalar, int N, typename El, bool h, bool o>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoord(uint i)
{
	return texCoords().at(i);
}

template<typename Scalar, int N, typename El, bool h, bool o>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoord(uint i) const
{
	return texCoords().at(i);
}

template<typename Scalar, int N, typename El, bool h, bool o>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoordMod(int i)
{
	return texCoords().atMod(i);
}

template<typename Scalar, int N, typename El, bool h, bool o>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoordMod(int i) const
{
	return texCoords().atMod(i);
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i)
{
	texCoords().set(t, i);
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar> >& list)
{
	texCoords().set(list);
}

template<typename Scalar, int N, typename El, bool h, bool o>
short& WedgeTexCoords<Scalar, N, El, h, o>::textureIndex()
{
	return texIndex();
}

template<typename Scalar, int N, typename El, bool h, bool o>
const short& WedgeTexCoords<Scalar, N, El, h, o>::textureIndex() const
{
	return texIndex();
}

template<typename Scalar, int N, typename El, bool h, bool o>
typename WedgeTexCoords<Scalar, N, El, h, o>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoordBegin()
{
	return texCoords().begin();
}

template<typename Scalar, int N, typename El, bool h, bool o>
typename WedgeTexCoords<Scalar, N, El, h, o>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoordEnd()
{
	return texCoords().end();
}

template<typename Scalar, int N, typename El, bool h, bool o>
typename WedgeTexCoords<Scalar, N, El, h, o>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoordBegin() const
{
	return texCoords().begin();
}

template<typename Scalar, int N, typename El, bool h, bool o>
typename WedgeTexCoords<Scalar, N, El, h, o>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoordEnd() const
{
	return texCoords().end();
}

template<typename Scalar, int N, typename El, bool h, bool o>
typename WedgeTexCoords<Scalar, N, El, h, o>::WedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoords()
{
	return texCoords().rangeIterator();
}

template<typename Scalar, int N, typename El, bool h, bool o>
typename WedgeTexCoords<Scalar, N, El, h, o>::ConstWedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N, El, h, o>::wedgeTexCoords() const
{
	return texCoords().rangeIterator();
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::resizeWedgeTexCoords(uint n) requires (N < 0)
{
	texCoords().resize(n);
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t) requires (N < 0)
{
	texCoords().pushBack(t);
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::insertWedgeTexCoord(
	uint                         i,
	const vcl::TexCoord<Scalar>& t) requires(N < 0)
{
	texCoords().insert(i, t);
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::eraseWedgeTexCoord(uint i) requires (N < 0)
{
	texCoords().erase(i);
}

template<typename Scalar, int N, typename El, bool h, bool o>
void WedgeTexCoords<Scalar, N, El, h, o>::clearWedgeTexCoord() requires (N < 0)
{
	texCoords().clear();
}

template<typename Scalar, int N, typename El, bool h, bool o>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, h, o>::importFrom(const Element& e)
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

template<typename Scalar, int N, typename El, bool h, bool o>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, h, o>::importWedgeTexCoordsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeTexCoord(i) = e.wedgeTexCoord(i).template cast<Scalar>();
	}
	texIndex() = e.textureIndex();
}

template<typename Scalar, int N, typename El, bool h, bool o>
short& WedgeTexCoords<Scalar, N, El, h, o>::texIndex()
{
	return data.template get<El>(this).texIndex;
}

template<typename Scalar, int N, typename El, bool h, bool o>
short WedgeTexCoords<Scalar, N, El, h, o>::texIndex() const
{
	return data.template get<El>(this).texIndex;
}

template<typename Scalar, int N, typename El, bool h, bool o>
RandomAccessContainer<vcl::TexCoord<Scalar>, N>& WedgeTexCoords<Scalar, N, El, h, o>::texCoords()
{
	return data.template get<El>(this).texCoords;
}

template<typename Scalar, int N, typename El, bool h, bool o>
const RandomAccessContainer<vcl::TexCoord<Scalar>, N>&
WedgeTexCoords<Scalar, N, El, h, o>::texCoords() const
{
	return data.template get<El>(this).texCoords;
}

} // namespace vcl::comp
