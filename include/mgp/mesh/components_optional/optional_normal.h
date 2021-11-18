/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_NORMAL_H
#define MGP_MESH_COMPONENTS_OPTIONAL_NORMAL_H

#include <mgp/space/point.h>

#include "optional_info.h"

namespace mgp::comp {

class OptionalNormalTrigger
{
};

template<typename Scalar, int N, typename T>
class OptionalNormal : public OptionalNormalTrigger, public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using NormalType = Point<Scalar, N>;
	const NormalType&        normal() const { return B::optCont().normal(thisId()); }
	NormalType&              normal() { return B::optCont().normal(thisId()); }

private:
	unsigned int thisId() const { return B::index((T*)this); }
};

template<typename Scalar, typename T>
class OptionalNormal3 : public OptionalNormal<Scalar, 3, T>
{
private:
	using B = OptionalInfo<T>;

public:
	using NormalType = Point3<Scalar>;
};

template<typename T>
class OptionalNormal3f : public OptionalNormal3<float, T>
{
};
template<typename T>
class OptionalNormal3d : public OptionalNormal3<double, T>
{
};

/**
 * Detector to check if a class has (inherits) OpionalNormal
 */

template<typename T>
using hasOptionalNormalT = std::is_base_of<OptionalNormalTrigger, T>;

template<typename U, typename T>
using ReturnIfHasOptionalNormal = typename std::enable_if<hasOptionalNormalT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalNormal()
{
	return hasOptionalNormalT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_OPTIONAL_NORMAL_H
