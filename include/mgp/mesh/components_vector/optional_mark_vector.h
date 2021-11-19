/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_MARK_VECTOR_H
#define MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_MARK_VECTOR_H

#include <mgp/mesh/components_optional/optional_mark.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalMarkVector
{
public:
	void clear() {}
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalMarkVector<T, std::enable_if_t<comp::hasOptionalMark<T>()>> :
		private OptionalGenericVector<int>
{
	using Base = OptionalGenericVector<int>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;
	bool       isMarkEnabled() const { return Base::isEnabled(); };
	void       enableMark(unsigned int size) { Base::enable(size); }
	void       disableMark() { Base::disable(); }
	int&       mark(unsigned int i) { return Base::at(i); }
	const int& mark(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_MARK_VECTOR_H
