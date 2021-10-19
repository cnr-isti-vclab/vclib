/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_COLOR_H
#define MGP_MESH_COMMON_COLOR_H

#include <mgp/space/color.h>

namespace mgp {
namespace common {

class Color
{
public:
	const mgp::Color& color() const { return c; }
	mgp::Color&       color() { return c; }

private:
	mgp::Color c;
};

} // namespace common
} // namespace mgp

#endif // MGP_MESH_COMMON_COLOR_H
