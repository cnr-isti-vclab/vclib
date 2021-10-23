/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_GENERIC_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_GENERIC_VECTOR_H

#include <assert.h>
#include <vector>

namespace mgp::mesh {

template<typename T>
class OptionalGenericVector
{
protected:
	bool isEnabled() const { return enabled; }
	void enable(unsigned int size)
	{
		enabled = true;
		vec.resize(size);
	}
	void disable()
	{
		enabled = false;
		vec.clear();
		vec.swap(std::vector<T>()); // force free memory
	}
	void resize(unsigned int size)
	{
		if (enabled)
			vec.resize(size);
	}
	void reserve(unsigned int size)
	{
		if (enabled)
			vec.reserve(size);
	}
	T& at(unsigned int i)
	{
		assert(enabled);
		return vec[i];
	}
	const T& at(unsigned int i) const
	{
		assert(enabled);
		return vec[i];
	}

private:
	bool           enabled = false;
	std::vector<T> vec;
};

} // namespace mgp::mesh

#endif // MGP_MESH_CONTAINER_OPTIONAL_GENERIC_VECTOR_H
