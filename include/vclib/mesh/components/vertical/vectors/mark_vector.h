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

#ifndef VCL_MESH_COMPONENTS_MARK_VECTOR_H
#define VCL_MESH_COMPONENTS_MARK_VECTOR_H

#include "../../concepts/mark.h"

#include "generic_component_vector.h"

namespace vcl::internal {

template<typename>
class MarkVector
{
public:
	void clear() {}
	void resize(uint) {}
	void reserve(uint) {}
	void compact(const std::vector<int>&) {}
	bool isMarkEnabled() const { return false; };
};

template<comp::HasOptionalMark T>
class MarkVector<T> : private GenericComponentVector<int>
{
	using Base = GenericComponentVector<int>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;

	bool isMarkEnabled() const { return Base::isEnabled(); };
	void enableMark(uint size) { Base::enable(size); }
	void disableMark() { Base::disable(); }

	int&       mark(uint i) { return Base::at(i); }
	const int& mark(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_MARK_VECTOR_H
