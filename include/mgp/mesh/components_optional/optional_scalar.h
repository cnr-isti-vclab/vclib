/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_SCALAR_H
#define MGP_MESH_COMPONENTS_OPTIONAL_SCALAR_H

#include "optional_info.h"

namespace mgp::components {

template<typename T>
class OptionalScalarTrigger
{
};

template<typename S, typename T>
class OptionalScalar : public OptionalScalarTrigger<T>, public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using ScalarType = S;
	const ScalarType& scalar() const { return B::contPtr->scalar(thisId()); }
	ScalarType&       scalar() { return B::contPtr->scalar(thisId()); }

private:
	unsigned int thisId() const { return ((T*) this)->id(); }
};

template<typename T>
class OptionalScalarf : public OptionalScalar<float, T>
{
};

template<typename T>
class OptionalScalard : public OptionalScalar<double, T>
{
};

/**
 * Detector to check if a class has (inherits) OpionalScalar
 */

template<typename T>
using hasOptionalScalarT = std::is_base_of<OptionalScalarTrigger<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalScalar = typename std::enable_if<hasOptionalScalarT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalScalar()
{
	return hasOptionalScalarT<T>::value;
}

} // namespace mgp::components

#endif //  MGP_MESH_COMPONENTS_OPTIONAL_SCALAR_H
