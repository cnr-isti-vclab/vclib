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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "../concepts/wedge_tex_coords.h"
#include "internal/generic_container.h"

#include <vclib/space/tex_coord.h>

namespace vcl::comp {

template<typename Scalar, int N>
class WedgeTexCoords :
		protected internal::GenericContainer<vcl::TexCoord<Scalar>, N>
{
	using Base = internal::GenericContainer<vcl::TexCoord<Scalar>, N>;

public:
	using WedgeTexCoordType = vcl::TexCoord<Scalar>;

	static const int WEDGE_TEX_COORD_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using WedgeTexCoordsIterator           = typename Base::GCIterator;
	using ConstWedgeTexCoordsIterator      = typename Base::ConstGCIterator;
	using WedgeTexCoordsRangeIterator      = typename Base::GCRangeIterator;
	using ConstWedgeTexCoordsRangeIterator = typename Base::ConstGCRangeIterator;

	WedgeTexCoords();

	/* Member functions */

	vcl::TexCoord<Scalar>&       wedgeTexCoord(uint i);
	const vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) const;

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const;

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i);
	void setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar>>& list);

	short& textureIndex();
	const short& textureIndex() const;

	constexpr bool isWedgeTexCoordsEnabled() const { return true; }

	/* Iterator Member functions */

	WedgeTexCoordsIterator           wedgeTexCoordBegin();
	WedgeTexCoordsIterator           wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const;
	WedgeTexCoordsRangeIterator      wedgeTexCoords();
	ConstWedgeTexCoordsRangeIterator wedgeTexCoords() const;

	// dummy member to discriminate between this component and the one simulated by the
	// FaceHalfEdgeReference component
	void __compWedgeTexCoords() const {};

protected:
	using WedgeTexCoordScalarType = Scalar;

	/* Member functions specific for vector */

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

	short texIndex;
};

} // namespace vcl::comp

#include "wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
