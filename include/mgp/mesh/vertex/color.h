/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_COLOR_H
#define MGP_MESH_VERTEX_COLOR_H

#include <mgp/space/color.h>

namespace mgp {
namespace vert {

class Color
{
public:
	const mgp::Color& color() const { return p; }
	mgp::Color&       color() { return p; }

private:
	mgp::Color p;
};

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_COLOR_H
