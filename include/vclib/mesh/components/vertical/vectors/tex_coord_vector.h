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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_VECTOR_H
#define VCL_MESH_COMPONENTS_TEX_COORD_VECTOR_H

#include "../../concepts/tex_coord.h"

#include "generic_component_vector.h"

namespace vcl::internal {

template<typename, typename = void>
class TexCoordVector
{
public:
	void clear() {}
	void reserve(uint) {}
	void resize(uint) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class TexCoordVector<T, std::enable_if_t<comp::hasOptionalTexCoord<T>()>> :
		private GenericComponentVector<typename T::TexCoordType>
{
	using TexCoordType = typename T::TexCoordType;
	using Base         = GenericComponentVector<TexCoordType>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;
	bool                isTexCoordEnabled() const { return Base::isEnabled(); };
	void                enableTexCoord(uint size) { Base::enable(size); }
	void                disableTexCoord() { Base::disable(); }
	TexCoordType&       texCoord(uint i) { return Base::at(i); }
	const TexCoordType& texCoord(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_TEX_COORD_VECTOR_H
