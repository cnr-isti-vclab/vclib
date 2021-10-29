/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_COLOR_H
#define MGP_MESH_COMMON_OPTIONAL_COLOR_H

#include <mgp/space/color.h>

#include "optional_info.h"

namespace mgp::common {

template<typename T>
class OptionalColor : public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	typedef mgp::Color ColorType;
	const mgp::Color& color() const { return B::containerPointer->color(((T*)this)->id()); }
	mgp::Color&       color() { return B::containerPointer->color(((T*)this)->id()); }
};

/**
 * Detector to check if a class has (inherits) OptionalColor
 */

template <typename  T>
using hasOptionalColorT =std::is_base_of<OptionalColor<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalColor = typename std::enable_if<hasOptionalColorT<U>::value, T>::type;

template <typename T>
bool constexpr hasOptionalColor() {return hasOptionalColorT<T>::value;}

} // namespace mgp::common

#endif // MGP_MESH_COMMON_OPTIONAL_COLOR_H
