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

#include "wedge_tex_coords.h"

#include <vclib/concepts/mesh/components/tex_coord.h>

namespace vcl::comp {

template<typename Scalar, int N, typename El, bool O>
bool WedgeTexCoords<Scalar, N, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Scalar, int N, typename El, bool O>
bool WedgeTexCoords<Scalar, N, El, O>::isWedgeTexCoordsEnabled() const
{
	return isEnabled();
}

template<typename Scalar, int N, typename El, bool O>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoord(uint i)
{
	return texCoords().at(i);
}

template<typename Scalar, int N, typename El, bool O>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoord(uint i) const
{
	return texCoords().at(i);
}

template<typename Scalar, int N, typename El, bool O>
vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordMod(int i)
{
	return texCoords().atMod(i);
}

template<typename Scalar, int N, typename El, bool O>
const vcl::TexCoord<Scalar>& WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordMod(int i) const
{
	return texCoords().atMod(i);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i)
{
	texCoords().set(t, i);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar> >& list)
{
	texCoords().set(list);
}

template<typename Scalar, int N, typename El, bool O>
short& WedgeTexCoords<Scalar, N, El, O>::textureIndex()
{
	return texIndex();
}

template<typename Scalar, int N, typename El, bool O>
short WedgeTexCoords<Scalar, N, El, O>::textureIndex() const
{
	return texIndex();
}

template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordBegin()
{
	return texCoords().begin();
}

template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::WedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordEnd()
{
	return texCoords().end();
}

template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordBegin() const
{
	return texCoords().begin();
}

template<typename Scalar, int N, typename El, bool O>
typename WedgeTexCoords<Scalar, N, El, O>::ConstWedgeTexCoordsIterator
WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoordEnd() const
{
	return texCoords().end();
}

template<typename Scalar, int N, typename El, bool O>
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoords()
{
	return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
}

template<typename Scalar, int N, typename El, bool O>
auto WedgeTexCoords<Scalar, N, El, O>::wedgeTexCoords() const
{
	return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
}

template<typename Scalar, int N, typename El, bool O>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, O>::importFrom(const Element& e)
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
				resize(e.vertexNumber());
				importWedgeTexCoordsFrom(e);
			}
		}
	}
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::resize(uint n) requires (N < 0)
{
	texCoords().resize(n);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::pushBack(const vcl::TexCoord<Scalar>& t) requires (N < 0)
{
	texCoords().pushBack(t);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::insert(uint i, const vcl::TexCoord<Scalar>& t)
	requires(N < 0)
{
	texCoords().insert(i, t);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::erase(uint i) requires (N < 0)
{
	texCoords().erase(i);
}

template<typename Scalar, int N, typename El, bool O>
void WedgeTexCoords<Scalar, N, El, O>::clear() requires (N < 0)
{
	texCoords().clear();
}

template<typename Scalar, int N, typename El, bool O>
template<typename Element>
void WedgeTexCoords<Scalar, N, El, O>::importWedgeTexCoordsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeTexCoord(i) = e.wedgeTexCoord(i).template cast<Scalar>();
	}
	texIndex() = e.textureIndex();
}

template<typename Scalar, int N, typename El, bool O>
short& WedgeTexCoords<Scalar, N, El, O>::texIndex()
{
	return Base::additionalData(this);
}

template<typename Scalar, int N, typename El, bool O>
short WedgeTexCoords<Scalar, N, El, O>::texIndex() const
{
	return Base::additionalData(this);
}

template<typename Scalar, int N, typename El, bool O>
Vector<vcl::TexCoord<Scalar>, N>& WedgeTexCoords<Scalar, N, El, O>::texCoords()
{
	return Base::container(this);
}

template<typename Scalar, int N, typename El, bool O>
const Vector<vcl::TexCoord<Scalar>, N>&
WedgeTexCoords<Scalar, N, El, O>::texCoords() const
{
	return Base::container(this);
}

template <typename T>
bool isWedgeTexCoordsEnabledOn(const T& element)
{
	return isComponentEnabledOn<WEDGE_TEX_COORDS>(element);
}

} // namespace vcl::comp
