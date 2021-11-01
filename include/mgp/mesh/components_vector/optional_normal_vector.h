/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_OPTIONAL_NORMAL_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_OPTIONAL_NORMAL_VECTOR_H

#include <mgp/mesh/components_optional/optional_normal.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalNormalVector
{
public:
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalNormalVector<T, std::enable_if_t<components::hasOptionalNormal<T>()>> :
		private OptionalGenericVector<typename T::NormalType>
{
	using NormalType = typename T::NormalType;
	using Base = OptionalGenericVector<NormalType>;

public:
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool              isNormalEnabled() const { return Base::isEnabled(); };
	void              enableNormal(unsigned int size) { Base::enable(size); }
	void              disableNormal() { Base::disable(); }
	NormalType&       normal(unsigned int i) { return Base::at(i); }
	const NormalType& normal(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENT_VECTOR_OPTIONAL_NORMAL_VECTOR_H
