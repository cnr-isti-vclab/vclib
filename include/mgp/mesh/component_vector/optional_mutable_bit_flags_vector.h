#ifndef MGP_MESH_COMPONENT_VECTOR_OPTIONAL_MUTABLE_BIT_FLAGS_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_OPTIONAL_MUTABLE_BIT_FLAGS_VECTOR_H

#include <mgp/mesh/common/optional_mutable_bit_flags.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class OptionalMutableBitFlagsVector
{
public:
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
};

template<typename T>
class OptionalMutableBitFlagsVector<
	T,
	std::enable_if_t<common::hasOptionalMutableBitFlags<T>()>> :
		private OptionalGenericVector<int>
{
	typedef OptionalGenericVector<int> Base;

public:
	using Base::reserve;
	using Base::resize;
	bool isMutableBitFlagsEnabled() { return Base::isEnabled(); };
	void enableMutableBitFlags(unsigned int size) { Base::enable(size); }
	void disableMutableBitFlags() { Base::disable(); }
	int& flags(unsigned int i) { return Base::at(i); }
	int& flags(unsigned int i) const { return const_cast<int&>(Base::at(i)); }
};

} // namespace mgp::mesh

#endif // MGP_MESH_COMPONENT_VECTOR_OPTIONAL_MUTABLE_BIT_FLAGS_VECTOR_H
