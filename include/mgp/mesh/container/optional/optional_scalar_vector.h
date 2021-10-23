/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_SCALAR_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_SCALAR_VECTOR_H

#include <mgp/mesh/common/optional_scalar.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class OptionalScalarVector
{
public:
	void resize(unsigned int) {}
};

template<typename T>
class OptionalScalarVector<T, std::enable_if_t<common::hasOptionalScalar<T>::value>> :
		private OptionalGenericVector<typename T::ScalarType>
{
	typedef typename T::ScalarType            ScalarType;
	typedef OptionalGenericVector<ScalarType> Base;

public:
	using Base::resize;
	bool              isScalarEnabled() { return Base::isEnabled(); };
	void              enableScalar(unsigned int size) { Base::enable(size); }
	void              disableScalar() { Base::disable(); }
	ScalarType&       scalar(unsigned int i) { return Base::at(i); }
	const ScalarType& scalar(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::mesh

#endif // MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_SCALAR_VECTOR_H
