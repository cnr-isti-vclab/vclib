/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VECTOR_GENERIC_VECTOR_H
#define MGP_MESH_COMPONENTS_VECTOR_GENERIC_VECTOR_H

#include <assert.h>
#include <vector>

namespace mgp::internal {

template<typename T>
class OptionalGenericVector
{
protected:
	bool isEnabled() const;
	void enable(unsigned int size);
	void disable();

	T& at(unsigned int i);
	const T& at(unsigned int i) const;

	void clear();
	void resize(unsigned int size);
	void reserve(unsigned int size);
	void compact(const std::vector<int>& newIndices);

private:
	bool           enabled = false;
	std::vector<T> vec;
};

} // namespace mgp::internal

#include "optional_generic_vector.cpp"

#endif // MGP_MESH_COMPONENTS_VECTOR_GENERIC_VECTOR_H
