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

#ifndef VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_WEDGE_COLORS_VECTOR_H
#define VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_WEDGE_COLORS_VECTOR_H

#include <vclib/mesh/components_optional/optional_wedge_colors.h>

#include "optional_generic_vector.h"

namespace vcl::internal {

template<typename, typename = void>
class OptionalWedgeColorsVector
{
public:
	void clear() {}
	void reserve(uint) {}
	void resize(uint) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalWedgeColorsVector<T, std::enable_if_t<comp::hasOptionalWedgeColors<T>()>> :
		private OptionalGenericVector<typename T::WedgeColorsContainer>
{
	using WedgeColorsContainer = typename T::WedgeColorsContainer;
	using Base                 = OptionalGenericVector<WedgeColorsContainer>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;
	bool                        isWedgeColorsEnabled() const { return Base::isEnabled(); };
	void                        enableWedgeColors(uint size) { Base::enable(size); }
	void                        disableWedgeColors() { Base::disable(); }
	WedgeColorsContainer&       wedgeColors(uint i) { return Base::at(i); }
	const WedgeColorsContainer& wedgeColors(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_WEDGE_COLORS_VECTOR_H
