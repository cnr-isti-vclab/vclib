/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_OPTIONAL_MUTABLE_BIT_FLAGS_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_OPTIONAL_MUTABLE_BIT_FLAGS_VECTOR_H

#include <mgp/mesh/components_optional/optional_mutable_bit_flags.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalMutableBitFlagsVector
{
public:
	void clear() {}
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalMutableBitFlagsVector<
	T,
	std::enable_if_t<components::hasOptionalMutableBitFlags<T>()>> :
		private OptionalGenericVector<int>
{
	using Base = OptionalGenericVector<int>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool isMutableBitFlagsEnabled() const { return Base::isEnabled(); };
	void enableMutableBitFlags(unsigned int size) { Base::enable(size); }
	void disableMutableBitFlags() { Base::disable(); }
	int& flags(unsigned int i) { return Base::at(i); }
	int& flags(unsigned int i) const { return const_cast<int&>(Base::at(i)); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENT_VECTOR_OPTIONAL_MUTABLE_BIT_FLAGS_VECTOR_H
