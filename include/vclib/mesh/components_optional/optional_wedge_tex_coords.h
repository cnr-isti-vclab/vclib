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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H

#include <array>
#include <vector>

#include "optional_info.h"

#include <vclib/space/tex_coord.h>

#include "../components/detection/wedge_tex_coords_detection.h"
#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalWedgeTexCoordsVector;
}

namespace vcl::comp {

template<typename Scalar, int N, typename T>
class OptionalWedgeTexCoords :
		public OptionalWedgeTexCoordsTriggerer,
		public virtual OptionalInfo<T>
{
	template<typename, typename>
	friend class OptionalWedgeTexCoordsVector;

private:
	using B = OptionalInfo<T>;
	uint thisId() const { return B::index((T*)this); }

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	using WedgeTexCoordType = vcl::TexCoord<Scalar>;

	// the AdjFacesContainer type will be array or vector, depending on N value
	using WedgeTexCoordsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>,
		typename std::vector<vcl::TexCoord<Scalar>>>::type;

	static const int WEDGE_TEX_COORD_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using WedgeTexCoordsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>::iterator,
		typename std::vector<vcl::TexCoord<Scalar>>::iterator>::type;

	using ConstWedgeTexCoordsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>::const_iterator,
		typename std::vector<vcl::TexCoord<Scalar>>::const_iterator>::type;

	using WedgeTexCoordsRangeIterator =
		RangeIterator<OptionalWedgeTexCoords, WedgeTexCoordsIterator>;
	using ConstWedgeTexCoordsRangeIterator =
		ConstRangeIterator<OptionalWedgeTexCoords, ConstWedgeTexCoordsIterator>;

	/** Constructor **/

	OptionalWedgeTexCoords();

	/** Member functions **/

	vcl::TexCoord<Scalar>&       wedgeTexCoord(uint i);
	const vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) const;

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const;

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i);
	void setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar>>& list);

	bool isWedgeTexCoordsEnabled() const;

	/** Iterator Member functions **/

	WedgeTexCoordsIterator           wedgeTexCoordBegin();
	WedgeTexCoordsIterator           wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const;
	WedgeTexCoordsRangeIterator      wedgeTexCoordIterator();
	ConstWedgeTexCoordsRangeIterator wedgeTexCoordIterator() const;

protected:
	using WedgeTexCoordScalarType = Scalar;

	/** Member functions specific for vector **/

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeWedgeTexCoords(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	insertWedgeTexCoord(uint i, const vcl::TexCoord<Scalar>& t);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseWedgeTexCoord(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearWedgeTexCoord();

	template <typename Element>
	void importFrom(const Element& e);

private:
	uint wedgeTexCoordsNumber() const;
};

} // namespace vcl::comp

#include "optional_wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H
