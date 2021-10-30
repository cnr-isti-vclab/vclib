/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_NORMAL_H
#define MGP_MESH_COMMON_OPTIONAL_NORMAL_H

#include <mgp/space/point.h>

#include "optional_info.h"

namespace mgp::common {

template <typename T>
class OptionalNormalTrigger {};

template<typename Scalar, int N, typename T>
class OptionalNormal : public OptionalNormalTrigger<T>, public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	typedef Point<Scalar, N> NormalType;
	const NormalType& normal() const { return B::contPtr->normal(((T*)this)->id()); }
	NormalType&       normal() { return B::contPtr->normal(((T*)this)->id()); }
};

template<typename Scalar, typename T>
class OptionalNormal3 : public OptionalNormal<Scalar, 3, T>
{
private:
	typedef OptionalInfo<T> B;
public:
	typedef Point3<Scalar> NormalType;
};

template<typename T>
class OptionalNormal3f : public OptionalNormal3<float, T> {};
template<typename T>
class OptionalNormal3d : public OptionalNormal3<double, T> {};


/**
 * Detector to check if a class has (inherits) OpionalNormal
 */

template <typename T>
using hasOptionalNormalT = std::is_base_of<OptionalNormalTrigger<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalNormal = typename std::enable_if<hasOptionalNormalT<U>::value, T>::type;

template <typename T>
bool constexpr hasOptionalNormal() {return hasOptionalNormalT<T>::value;}

} // namespace mgp::common


#endif // MGP_MESH_COMMON_OPTIONAL_NORMAL_H
