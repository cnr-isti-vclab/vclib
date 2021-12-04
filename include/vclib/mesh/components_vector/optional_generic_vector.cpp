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

#include "optional_generic_vector.h"

#include <vclib/misc/compactness.h>

namespace vcl::internal {

template<typename T>
bool OptionalGenericVector<T>::isEnabled() const
{
	return enabled;
}

template<typename T>
void OptionalGenericVector<T>::enable(uint size)
{
	enabled = true;
	vec.resize(size);
}

template<typename T>
void OptionalGenericVector<T>::disable()
{
	enabled = false;
	vec.clear();
	std::vector<T> tmp;
	vec.swap(tmp); // force free memory
}

template<typename T>
T& OptionalGenericVector<T>::at(uint i)
{
	assert(enabled);
	return vec[i];
}

template<typename T>
const T& OptionalGenericVector<T>::at(uint i) const
{
	assert(enabled);
	return vec[i];
}

/**
 * @brief OptionalGenericVector::clear clears the vector of the optional component,
 * without disabling it.
 */
template<typename T>
void OptionalGenericVector<T>::clear()
{
	vec.clear();
}

template<typename T>
void OptionalGenericVector<T>::resize(uint size)
{
	if (enabled)
		vec.resize(size);
}

template<typename T>
void OptionalGenericVector<T>::reserve(uint size)
{
	if (enabled)
		vec.reserve(size);
}

template<typename T>
void OptionalGenericVector<T>::compact(const std::vector<int>& newIndices)
{
	if (enabled)
		compactVector(vec, newIndices);
}

}
