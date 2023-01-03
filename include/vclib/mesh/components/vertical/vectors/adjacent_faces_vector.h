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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_VECTOR_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_VECTOR_H

#include "../../concepts/adjacent_faces.h"

#include "generic_component_vector.h"

namespace vcl::internal {

// the AdjFacesContainer type will be array or vector, depending on N value
template<typename F, int N>
using AdjFacesContainer = typename std::conditional<
	(N >= 0),
	typename std::array<F*, N >= 0 ? N : 0>,
	typename std::vector<F*>>::type;

template<typename>
class AdjacentFacesVector
{
public:
	void clear() {}
	void resize(uint) {}
	void reserve(uint) {}
	void compact(const std::vector<int>&) {}
	bool isAdjacentFacesEnabled() const { return false; };
};

template<comp::HasOptionalAdjacentFaces T>
class AdjacentFacesVector<T> :
		private GenericComponentVector<
			AdjFacesContainer<typename T::AdjacentFaceType, T::ADJ_FACE_NUMBER>>
{
private:
	using Container = AdjFacesContainer<typename T::AdjacentFaceType, T::ADJ_FACE_NUMBER>;
	using Base      = GenericComponentVector<Container>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;

	bool isAdjacentFacesEnabled() const { return Base::isEnabled(); };
	void enableAdjacentFaces(uint size) { Base::enable(size); }
	void disableAdjacentFaces() { Base::disable(); }

	Container&       adjFaces(uint i) { return Base::at(i); }
	const Container& adjFaces(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_VECTOR_H
