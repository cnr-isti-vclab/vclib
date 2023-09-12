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

/**
 * @brief Allows to access directly to a custom component.
 *
 * The class allows to access a custom component stored in a Contaner of
 * Elements without having to use the Container itself and avoiding copies, and
 * it can be used as a normal std::vector. The class stores references to the
 * custom components, therefore it allows to modify them.
 *
 * It is meant to be created by a Container, that constructs it from the vector
 * of custom components and then returns it to the user.
 *
 * @note A CustomComponentVectorHandle object is meant to be used to access the
 * custom components. It does not make sense to modify the size of the container
 * or to add or remove elements. Member functions that modify the size of the
 * container are not implemented.
 *
 * @note If the Element Container is modified after the creation of a
 * CustomComponentVectorHandle, the CustomComponentVectorHandle is not updated
 * and still contains the old references to the custom components (that may be
 * invalidated).
 *
 * @tparam T: The type of the custom component.
 */
template<typename T>
class CustomComponentVectorHandle
{
public:
	class Iterator : public std::vector<std::reference_wrapper<T>>::iterator
	{
		using Base = std::vector<std::reference_wrapper<T>>::iterator;
	public:
		Iterator(Base it) : Base(it) {}
		using value_type = T;
		using reference = value_type&;
		using pointer = value_type*;
		reference operator*() const { return Base::operator*().get(); }
		pointer operator->() const { return &(Base::operator*().get()); }
	};

	class ConstIterator :
		public std::vector<std::reference_wrapper<T>>::const_iterator
	{
		using Base = std::vector<std::reference_wrapper<T>>::const_iterator;

	public:
		ConstIterator(Base it) : Base(it) {}
		using value_type = T;
		using reference = const value_type&;
		using pointer = const value_type*;
		reference operator*() const { return Base::operator*().get(); }
		pointer operator->() const { return &(Base::operator*().get()); }
	};

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

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

private:
	std::vector<std::reference_wrapper<T>> v;
};

template<typename T>
using ConstCustomComponentVectorHandle = CustomComponentVectorHandle<const T>;

} // namespace vcl

#include "custom_component_vector_handle.cpp"

#endif // VCL_MESH_CONTAINERS_CUSTOM_COMPONENT_VECTOR_HANDLE_H
