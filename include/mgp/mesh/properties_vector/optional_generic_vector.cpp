/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_generic_vector.h"

#include <mgp/misc/compactness.h>

namespace mgp::internal {

template<typename T>
bool OptionalGenericVector<T>::isEnabled() const
{
	return enabled;
}

template<typename T>
void OptionalGenericVector<T>::enable(unsigned int size)
{
	enabled = true;
	vec.resize(size);
}

template<typename T>
void OptionalGenericVector<T>::disable()
{
	enabled = false;
	vec.clear();
	vec.swap(std::vector<T>()); // force free memory
}

template<typename T>
T& OptionalGenericVector<T>::at(unsigned int i)
{
	assert(enabled);
	return vec[i];
}

template<typename T>
const T& OptionalGenericVector<T>::at(unsigned int i) const
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
void OptionalGenericVector<T>::resize(unsigned int size)
{
	if (enabled)
		vec.resize(size);
}

template<typename T>
void OptionalGenericVector<T>::reserve(unsigned int size)
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
