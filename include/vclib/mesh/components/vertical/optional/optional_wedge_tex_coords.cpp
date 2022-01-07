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

#include "optional_wedge_tex_coords.h"

#include <algorithm>

namespace vcl::comp {

template<typename Scalar, int N, typename T>
OptionalWedgeTexCoords<Scalar, N, T>::OptionalWedgeTexCoords()
{
}

template<typename Scalar, int N, typename T>
vcl::TexCoord<Scalar>& OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoord(uint i)
{
	assert(i < wedgeTexCoordsNumber());
	return B::optCont().wedgeTexCoords(thisId())[i];
}

template<typename Scalar, int N, typename T>
const vcl::TexCoord<Scalar>&
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoord(uint i) const
{
	assert(i < wedgeTexCoordsNumber());
	return B::optCont().wedgeTexCoords(thisId())[i];
}

template<typename Scalar, int N, typename T>
vcl::TexCoord<Scalar>& OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordMod(int i)
{
	int n = wedgeTexCoordsNumber();
	return B::optCont().wedgeTexCoords(thisId())[(i % n + n) % n];
}

template<typename Scalar, int N, typename T>
const vcl::TexCoord<Scalar>& OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordMod(int i) const
{
	int n = wedgeTexCoordsNumber();
	return B::optCont().wedgeTexCoords(thisId())[(i % n + n) % n];
}

template<typename Scalar, int N, typename T>
void OptionalWedgeTexCoords<Scalar, N, T>::setWedgeTexCoord(
	const vcl::TexCoord<Scalar>& t,
	uint                 i)
{
	assert(i < wedgeTexCoordsNumber());
	B::optCont().wedgeTexCoords(thisId())[i] = t;
}

template<typename Scalar, int N, typename T>
void OptionalWedgeTexCoords<Scalar, N, T>::setWedgeTexCoords(
	const std::vector<vcl::TexCoord<Scalar>>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& t : list) {
			setWedgeTexCoord(t, i);
			++i;
		}
	}
	else {
		B::optCont().wedgeTexCoords(thisId()) = list;
	}
}

template<typename Scalar, int N, typename T>
bool OptionalWedgeTexCoords<Scalar, N, T>::isWedgeTexCoordsEnabled() const
{
	return B::optCont().isWedgeTexCoordsEnabled();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::WedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordBegin()
{
	return B::optCont().wedgeTexCoords(thisId()).begin();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::WedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordEnd()
{
	return B::optCont().wedgeTexCoords(thisId()).end();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::ConstWedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordBegin() const
{
	return B::optCont().wedgeTexCoords(thisId()).begin();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::ConstWedgeTexCoordsIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordEnd() const
{
	return B::optCont().wedgeTexCoords(thisId()).end();
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::WedgeTexCoordsRangeIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoords()
{
	return WedgeTexCoordsRangeIterator(
		*this,
		&OptionalWedgeTexCoords::wedgeTexCoordBegin,
		&OptionalWedgeTexCoords::wedgeTexCoordEnd);
}

template<typename Scalar, int N, typename T>
typename OptionalWedgeTexCoords<Scalar, N, T>::ConstWedgeTexCoordsRangeIterator
OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoords() const
{
	return ConstWedgeTexCoordsRangeIterator(
		*this,
		&OptionalWedgeTexCoords::wedgeTexCoordBegin,
		&OptionalWedgeTexCoords::wedgeTexCoordEnd);
}

template<typename Scalar, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalWedgeTexCoords<Scalar, N, T>::resizeWedgeTexCoords(uint n)
{
	B::optCont().wedgeTexCoords(thisId()).resize(n);
}

template<typename Scalar, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalWedgeTexCoords<Scalar, N, T>::pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t)
{
	B::optCont().wedgeTexCoords(thisId()).push_back(t);
}

template<typename Scalar, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalWedgeTexCoords<Scalar, N, T>::insertWedgeTexCoord(
	uint                 i,
	const vcl::TexCoord<Scalar>& t)
{
	assert(i < wedgeTexCoordsNumber());
	B::optCont().wedgeTexCoords(thisId()).insert(
		B::optCont().wedgeTexCoords(thisId()).begin() + i, t);
}

template<typename Scalar, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void)
OptionalWedgeTexCoords<Scalar, N, T>::eraseWedgeTexCoord(uint i)
{
	assert(i < wedgeTexCoordsNumber());
	B::optCont().wedgeTexCoords(thisId()).erase(B::optCont().wedgeTexCoords(thisId()).begin() + i);
}

template<typename Scalar, int N, typename T>
template<int M>
VCL_ENABLE_IF(M < 0, void) OptionalWedgeTexCoords<Scalar, N, T>::clearWedgeTexCoord()
{
	B::optCont().wedgeTexCoords(thisId()).clear();
}

template<typename Scalar, int N, typename T>
template<typename Element>
void OptionalWedgeTexCoords<Scalar, N, T>::importFrom(const Element& e)
{
	if constexpr (hasWedgeTexCoords<Element>()) {
		if (isWedgeTexCoordsEnabled() && isWedgeTexCoordsEnabledOn(e)) {
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

template<typename Scalar, int N, typename T>
template<typename Element>
void OptionalWedgeTexCoords<Scalar, N, T>::importWedgeTexCoordsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeTexCoord(i) = e.wedgeTexCoord(i);
	}
}

template<typename Scalar, int N, typename T>
uint OptionalWedgeTexCoords<Scalar, N, T>::wedgeTexCoordsNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().wedgeTexCoords(thisId()).size();
	}
}

} // namespace vcl::comp