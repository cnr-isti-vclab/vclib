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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H

#include <array>
#include <vector>

#include <vclib/iterators/range_iterator.h>

#include "../vertical_component.h"

#include <vclib/space/tex_coord.h>

#include "../../concepts/wedge_tex_coords.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalWedgeTexCoordsVector;
}

namespace vcl::comp {

template<typename Scalar, int N, typename T>
class OptionalWedgeTexCoords : public virtual VerticalComponent<T>
{
	template<typename, typename>
	friend class OptionalWedgeTexCoordsVector;

private:
	using B = VerticalComponent<T>;
	uint thisId() const { return B::index((T*)this); }

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// possibility to access to the WedgeTexCoords class, whether is optional or not.
	using WedgeTexCoords = OptionalWedgeTexCoords;

	using WedgeTexCoordType = vcl::TexCoord<Scalar>;

	// the WedgeTexCoordsContainer is a pair Container - short
	// the Container will be array or vector, depending on N value
	// short is the index of the texture used by the current container of wedge texcoords
	using WedgeTexCoordsContainer =
		std::pair<
			typename std::conditional<(N >= 0),
				typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>,
				typename std::vector<vcl::TexCoord<Scalar>>>::type,
			short>;

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

	short& textureIndex();
	const short& textureIndex() const;

	bool isWedgeTexCoordsEnabled() const;

	/** Iterator Member functions **/

	WedgeTexCoordsIterator           wedgeTexCoordBegin();
	WedgeTexCoordsIterator           wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const;
	WedgeTexCoordsRangeIterator      wedgeTexCoords();
	ConstWedgeTexCoordsRangeIterator wedgeTexCoords() const;

	// dummy member to discriminate between non-optional and optional component
	void __optionalWedgeTexCoords() const {};

	// dummy member to discriminate between this component and the one simulated by the
	// FaceHalfEdgeReference component
	void __compWedgeTexCoords() const {};

protected:
	using WedgeTexCoordScalarType = Scalar;

	/** Member functions specific for vector **/

	void resizeWedgeTexCoords(uint n) requires (N < 0);
	void pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t) requires (N < 0);
	void insertWedgeTexCoord(uint i, const vcl::TexCoord<Scalar>& t) requires (N < 0);
	void eraseWedgeTexCoord(uint i) requires (N < 0);
	void clearWedgeTexCoord() requires (N < 0);

	template <typename Element>
	void importFrom(const Element& e);

private:
	template<typename Element>
	void importWedgeTexCoordsFrom(const Element& e);

	uint wedgeTexCoordsNumber() const;
};

} // namespace vcl::comp

#include "optional_wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H
