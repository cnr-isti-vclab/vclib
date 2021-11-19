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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "generic_container.h"

#include <vclib/space/tex_coord.h>

namespace vcl::comp {

class WedgeTexCoordsTriggerer
{
};

template<typename Scalar, int N>
class WedgeTexCoords :
		protected GenericContainer<vcl::TexCoord<Scalar>, N>,
		public WedgeTexCoordsTriggerer
{
	using Base = GenericContainer<vcl::TexCoord<Scalar>, N>;

public:
	static const int WEDGE_TEXCOORD_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using WedgeTexCoordsIterator           = typename Base::GCIterator;
	using ConstWedgeTexCoordsIterator      = typename Base::ConstGCIterator;
	using WedgeTexCoordsRangeIterator      = typename Base::GCRangeIterator;
	using ConstWedgeTexCoordsRangeIterator = typename Base::ConstGCRangeIterator;

	WedgeTexCoords() : Base() {};

	/** Member functions **/

	vcl::TexCoord<Scalar>&       wedgeTexCoord(unsigned int i) { return Base::at(i); }
	const vcl::TexCoord<Scalar>& wedgeTexCoord(unsigned int i) const { return Base::at(i); }

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i) { return Base::atMod(i); }
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const { return Base::atMod(i); }

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, unsigned int i) { Base::set(t, i); }
	void setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar>>& list) { Base::set(list); }

	/** Iterator Member functions **/

	WedgeTexCoordsIterator           wedgeTexCoordBegin() { return Base::begin(); }
	WedgeTexCoordsIterator           wedgeTexCoordEnd() { return Base::end(); }
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const { return Base::begin(); }
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const { return Base::end(); }
	WedgeTexCoordsRangeIterator      wedgeTexCoordIterator() { return Base::rangeIterator(); }
	ConstWedgeTexCoordsRangeIterator wedgeTexCoordIterator() const { return Base::rangeIterator(); }

protected:
	using WedgeTexCoordScalarType = Scalar;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeWedgeTexCoords(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t)
	{
		Base::pushBack(t);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void>
	insertWedgeTexCoord(unsigned int i, const vcl::TexCoord<Scalar>& t)
	{
		Base::insert(i, t);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseWedgeTexCoord(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearWedgeTexCoord()
	{
		Base::clear();
	}
};

template<typename T>
using hasWedgeTexCoordsT = std::is_base_of<WedgeTexCoordsTriggerer, T>;

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return hasWedgeTexCoordsT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
