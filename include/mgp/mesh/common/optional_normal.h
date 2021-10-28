/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_NORMAL_H
#define MGP_MESH_COMMON_OPTIONAL_NORMAL_H

#include <mgp/space/point.h>

#include "optional_info.h"

namespace mgp::common {

template<typename Scalar, int N, typename T>
class OptionalNormal : public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	typedef Point<Scalar, N> NormalType;
	const NormalType& normal() const { return B::containerPointer->normal(B::id()); }
	NormalType&       normal() { return B::containerPointer->normal(B::id()); }

	void __optional_normal__() {};
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

template<typename T>
using hasOptionalNormal_t = decltype(std::declval<T&>().__optional_normal__());

template <typename T>
using hasOptionalNormal = typename detector<hasOptionalNormal_t, void, T>::type;

template<typename U, typename T>
using ReturnIfHasOptionalNormal = typename std::enable_if<hasOptionalNormal<U>::value, T>::type;

} // namespace mgp::common


#endif // MGP_MESH_COMMON_OPTIONAL_NORMAL_H
