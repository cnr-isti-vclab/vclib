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

#ifndef VCL_MESH_COMPONENTS_VECTOR_GENERIC_VECTOR_H
#define VCL_MESH_COMPONENTS_VECTOR_GENERIC_VECTOR_H

#include <assert.h>
#include <vector>

#include <vclib/misc/vcl_types.h>

namespace vcl::internal {

template<typename T>
class OptionalGenericVector
{
protected:
	bool isEnabled() const;
	void enable(uint size);
	void disable();

	T& at(uint i);
	const T& at(uint i) const;

	void clear();
	void resize(uint size);
	void reserve(uint size);
	void compact(const std::vector<int>& newIndices);

private:
	bool           enabled = false;
	std::vector<T> vec;
};

} // namespace vcl::internal

#include "optional_generic_vector.cpp"

#endif // VCL_MESH_COMPONENTS_VECTOR_GENERIC_VECTOR_H
