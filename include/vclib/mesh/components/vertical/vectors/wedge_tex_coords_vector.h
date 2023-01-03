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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_VECTOR_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_VECTOR_H

#include "../../concepts/wedge_tex_coords.h"

#include "generic_component_vector.h"

namespace vcl::internal {

// the WedgeTexCoordsContainer is a pair Container - short
// the Container will be array or vector, depending on N value
// short is the index of the texture used by the current container of wedge texcoords
template<typename TCT, int N>
using WedgeTexCoordsContainer =
	std::pair<
		typename std::conditional<(N >= 0),
				typename std::array<TCT, N >= 0 ? N : 0>,
				typename std::vector<TCT>>::type,
		short>;

template<typename>
class WedgeTexCoordsVector
{
public:
	void clear() {}
	void reserve(uint) {}
	void resize(uint) {}
	void compact(const std::vector<int>&) {}
	bool isWedgeTexCoordsEnabled() const { return false; };
};

template<comp::HasOptionalWedgeTexCoords T>
class WedgeTexCoordsVector<T> :
		private GenericComponentVector<
			WedgeTexCoordsContainer<
				typename T::WedgeTexCoordType, T::WEDGE_TEX_COORD_NUMBER>>
{
	using Container = WedgeTexCoordsContainer<typename T::WedgeTexCoordType, T::WEDGE_TEX_COORD_NUMBER>;
	using Base      = GenericComponentVector<Container>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;

	bool isWedgeTexCoordsEnabled() const { return Base::isEnabled(); };
	void enableWedgeTexCoords(uint size) { Base::enable(size); }
	void disableWedgeTexCoords() { Base::disable(); }

	Container&       wedgeTexCoords(uint i) { return Base::at(i); }
	const Container& wedgeTexCoords(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_VECTOR_H
