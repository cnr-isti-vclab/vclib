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

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::init()
{
	texIndex() = 0;
}

template<typename Scalar, int N, typename El, bool h>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoord(uint i)
{
	return texCoords().at(i);
}

template<typename Scalar, int N, typename El, bool h>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoord(uint i) const
{
	return texCoords().at(i);
}

template<typename Scalar, int N, typename El, bool h>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoordMod(int i)
{
	return texCoords().atMod(i);
}

template<typename Scalar, int N, typename El, bool h>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoordMod(int i) const
{
	return texCoords().atMod(i);
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i)
{
	texCoords().set(t, i);
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar> >& list)
{
	texCoords().set(list);
}

template<typename Scalar, int N, typename El, bool h>
short& WedgeTexCoords<Scalar, N, El, h>::textureIndex()
{
	return texIndex();
}

template<typename Scalar, int N, typename El, bool h>
const short& WedgeTexCoords<Scalar, N, El, h>::textureIndex() const
{
	return texIndex();
}

template<typename Scalar, int N, typename El, bool h>
typename WedgeTexCoords<Scalar, N, El, h>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoordBegin()
{
	return texCoords().begin();
}

template<typename Scalar, int N, typename El, bool h>
typename WedgeTexCoords<Scalar, N, El, h>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoordEnd()
{
	return texCoords().end();
}

template<typename Scalar, int N, typename El, bool h>
typename WedgeTexCoords<Scalar, N, El, h>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoordBegin() const
{
	return texCoords().begin();
}

template<typename Scalar, int N, typename El, bool h>
typename WedgeTexCoords<Scalar, N, El, h>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoordEnd() const
{
	return texCoords().end();
}

template<typename Scalar, int N, typename El, bool h>
typename WedgeTexCoords<Scalar, N, El, h>::WedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoords()
{
	return texCoords().rangeIterator();
}

template<typename Scalar, int N, typename El, bool h>
typename WedgeTexCoords<Scalar, N, El, h>::ConstWedgeTexCoordsRangeIterator
WedgeTexCoords<Scalar, N, El, h>::wedgeTexCoords() const
{
	return texCoords().rangeIterator();
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::resizeWedgeTexCoords(uint n) requires (N < 0)
{
	texCoords().resize(n);
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t) requires (N < 0)
{
	texCoords().pushBack(t);
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::insertWedgeTexCoord(
	uint                         i,
	const vcl::TexCoord<Scalar>& t) requires(N < 0)
{
	texCoords().insert(i, t);
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::eraseWedgeTexCoord(uint i) requires (N < 0)
{
	texCoords().erase(i);
}

template<typename Scalar, int N, typename El, bool h>
void WedgeTexCoords<Scalar, N, El, h>::clearWedgeTexCoord() requires (N < 0)
{
	texCoords().clear();
}

template<typename Scalar, int N, typename El, bool h>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, h>::importFrom(const Element& e)
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

template<typename Scalar, int N, typename El, bool h>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, h>::importWedgeTexCoordsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeTexCoord(i) = e.wedgeTexCoord(i).template cast<Scalar>();
	}
	texIndex() = e.textureIndex();
}

template<typename Scalar, int N, typename El, bool h>
short& WedgeTexCoords<Scalar, N, El, h>::texIndex()
{
	return data.template get<El>(this).texIndex;
}

template<typename Scalar, int N, typename El, bool h>
short WedgeTexCoords<Scalar, N, El, h>::texIndex() const
{
	return data.template get<El>(this).texIndex;
}

template<typename Scalar, int N, typename El, bool h>
RandomAccessContainer<vcl::TexCoord<Scalar>, N>& WedgeTexCoords<Scalar, N, El, h>::texCoords()
{
	return data.template get<El>(this).texCoords;
}

template<typename Scalar, int N, typename El, bool h>
const RandomAccessContainer<vcl::TexCoord<Scalar>, N>&
WedgeTexCoords<Scalar, N, El, h>::texCoords() const
{
	return data.template get<El>(this).texCoords;
}

} // namespace vcl::comp
