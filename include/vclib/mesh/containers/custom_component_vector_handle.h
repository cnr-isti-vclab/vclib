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

#ifndef VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H
#define VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H

#include <any>
#include <vector>

#include <vclib/types.h>

namespace vcl {

template<typename T>
class CustomComponentVectorHandle
{
public:
	using iterator = typename std::vector<std::reference_wrapper<T>>::iterator;
	using const_iterator = typename std::vector<std::reference_wrapper<T>>::const_iterator;

	CustomComponentVectorHandle();
	CustomComponentVectorHandle(std::vector<std::any>& cc);

	T& at(uint i);
	const T& at(uint i) const;

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

	uint size() const;

	T& operator[](uint i);
	const T& operator[](uint i) const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

private:
	std::vector<std::reference_wrapper<T>> v;
};

template<typename T>
using ConstCustomComponentVectorHandle = CustomComponentVectorHandle<const T>;

} // namespace vcl

#include "custom_component_vector_handle.cpp"

#endif // VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H
