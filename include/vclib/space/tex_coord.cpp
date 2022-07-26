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

#include "tex_coord.h"
#include <assert.h>

namespace vcl {

template<typename Scalar>
TexCoord<Scalar>::TexCoord() : coord(0, 0), n(0)
{
}

template<typename Scalar>
template<typename S>
TexCoord<S> TexCoord<Scalar>::cast() const
{
	if constexpr (std::is_same<Scalar, S>::value) {
		return *this;
	}
	else {
		TexCoord<S> tmp;
		tmp.coord = coord.template cast<S>();
		tmp.n = n;
		return tmp;
	}
}

template<typename Scalar>
Scalar TexCoord<Scalar>::u() const
{
	return coord.x();
}

template<typename Scalar>
Scalar TexCoord<Scalar>::v() const
{
	return coord.y();
}

template<typename Scalar>
Scalar& TexCoord<Scalar>::u()
{
	return coord.x();
}

template<typename Scalar>
Scalar& TexCoord<Scalar>::v()
{
	return coord.y();
}

template<typename Scalar>
void TexCoord<Scalar>::setU(Scalar s)
{
	assert(s >= 0 && s <= 1);
	coord.x() = s;
}

template<typename Scalar>
void TexCoord<Scalar>::setV(Scalar s)
{
	assert(s >= 0 && s <= 1);
	coord.y() = s;
}

template<typename Scalar>
void TexCoord<Scalar>::set(Scalar u, Scalar v)
{
	assert(u >= 0 && u <= 1);
	assert(v >= 0 && v <= 1);
	coord.x() = u;
	coord.y() = v;
}

template<typename Scalar>
short TexCoord<Scalar>::nTexture() const
{
	return n;
}

template<typename Scalar>
short& TexCoord<Scalar>::nTexture()
{
	return n;
}

template<typename Scalar>
Scalar& TexCoord<Scalar>::operator()(uint i)
{
	return coord[i];
}

template<typename Scalar>
const Scalar& TexCoord<Scalar>::operator()(uint i) const
{
	return coord[i];
}

template<typename Scalar>
Scalar& TexCoord<Scalar>::operator[](uint i)
{
	return coord[i];
}

template<typename Scalar>
const Scalar& TexCoord<Scalar>::operator[](uint i) const
{
	return coord[i];
}

} // namespace vcl
