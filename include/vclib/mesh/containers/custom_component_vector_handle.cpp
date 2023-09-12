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

#include "custom_component_vector_handle.h"

namespace vcl {

template<typename T>
CustomComponentVectorHandle<T>::CustomComponentVectorHandle()
{
}

template<typename T>
CustomComponentVectorHandle<T>::CustomComponentVectorHandle(
	std::vector<std::any>& cc)
{
	v.reserve(cc.size());
	for (uint i = 0; i < cc.size(); ++i) {
		v.push_back(std::any_cast<T&>(cc[i]));
	}
}

template<typename T>
T& CustomComponentVectorHandle<T>::at(uint i)
{
	return v[i].get();
}

template<typename T>
const T& CustomComponentVectorHandle<T>::at(uint i) const
{
	return v[i].get();
}

template<typename T>
T& CustomComponentVectorHandle<T>::front()
{
	return v.begin()->get();
}

template<typename T>
const T& CustomComponentVectorHandle<T>::front() const
{
	return v.begin()->get();
}

template<typename T>
T& CustomComponentVectorHandle<T>::back()
{
	return std::prev(v.end())->get();
}

template<typename T>
const T& CustomComponentVectorHandle<T>::back() const
{
	return std::prev(v.end())->get();
}

template<typename T>
uint CustomComponentVectorHandle<T>::size() const
{
	return v.size();
}

template<typename T>
T& CustomComponentVectorHandle<T>::operator[](uint i)
{
	return v[i].get();
}

template<typename T>
const T& CustomComponentVectorHandle<T>::operator[](uint i) const
{
	return v[i].get();
}

template<typename T>
auto CustomComponentVectorHandle<T>::begin() -> Iterator
{
	return Iterator(v.begin());
}

template<typename T>
auto CustomComponentVectorHandle<T>::end() -> Iterator
{
	return Iterator(v.end());
}

template<typename T>
auto CustomComponentVectorHandle<T>::begin() const -> ConstIterator
{
	return ConstIterator(v.begin());
}

template<typename T>
auto CustomComponentVectorHandle<T>::end() const -> ConstIterator
{
	return ConstIterator(v.end());
}

} // namespace vcl
