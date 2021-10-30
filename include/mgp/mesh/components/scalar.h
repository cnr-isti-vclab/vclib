/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_SCALAR_H
#define MGP_MESH_COMPONENTS_SCALAR_H

#include <type_traits>

namespace mgp::components {

class ScalarTrigger
{
};

template<typename T>
class Scalar : public ScalarTrigger
{
	typedef T ScalarType;

	const ScalarType& scalar() const { return s; }
	ScalarType&       scalar() { return s; }

private:
	ScalarType s;
};

typedef Scalar<float>  Scalarf;
typedef Scalar<double> Scalard;

/**
 * Detector to check if a class has (inherits) Scalar
 */

template<typename T>
using hasScalarT = std::is_base_of<ScalarTrigger, T>;

template<typename T>
bool constexpr hasScalar()
{
	return hasScalarT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_SCALAR_H
