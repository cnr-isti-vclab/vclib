/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_COLOR_H
#define MGP_MESH_COMMON_OPTIONAL_COLOR_H

#include <mgp/space/color.h>

#include "optional_info.h"

namespace mgp {
namespace common {

template<typename T>
class OptionalColor : public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	typedef mgp::Color ColorType;
	const mgp::Color& color() const { return B::containerPointer->color(B::id()); }
	mgp::Color&       color() { return B::containerPointer->colors(B::id()); }

	void __optional_color__() {} // dummy member used just for detection of an OpionalColor object
};

} // namespace common
} // namespace mgp

#endif // MGP_MESH_COMMON_OPTIONAL_COLOR_H
