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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "concepts/wedge_tex_coords.h"
#include "internal/component_data.h"

#include <vclib/misc/random_access_container.h>
#include <vclib/space/tex_coord.h>

namespace vcl::comp {

template<
	typename Scalar,
	int N,
	typename ElementType = void,
	bool horizontal      = true,
	bool optional        = false>
class WedgeTexCoords
{
	using ThisType = WedgeTexCoords<Scalar, N, ElementType, horizontal, optional>;

	struct WTCData {
		RandomAccessContainer<vcl::TexCoord<Scalar>, N> texCoords;
		short texIndex;
	};

	using Base = RandomAccessContainer<vcl::TexCoord<Scalar>, N>;
public:
	using DataValueType = WTCData; // data that the component stores internally (or vertically)
	using WedgeTexCoordsComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !horizontal;
	static const bool IS_OPTIONAL = optional;

	static const int WEDGE_TEX_COORD_NUMBER = Base::CONTAINER_SIZE;

	using WedgeTexCoordType = vcl::TexCoord<Scalar>;

	/* Iterator Types declaration */

	using WedgeTexCoordsIterator           = typename Base::Iterator;
	using ConstWedgeTexCoordsIterator      = typename Base::ConstIterator;
	using WedgeTexCoordsRangeIterator      = typename Base::RACRangeIterator;
	using ConstWedgeTexCoordsRangeIterator = typename Base::RACConstRangeIterator;

	void init();

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

	short& texIndex();
	short texIndex() const;
	RandomAccessContainer<vcl::TexCoord<Scalar>, N>& texCoords();
	const RandomAccessContainer<vcl::TexCoord<Scalar>, N>& texCoords() const;

	internal::ComponentData<DataValueType, horizontal> data;
};

} // namespace vcl::comp

#include "wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
