/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H

#include <vector>

#include "assert.h"

namespace mgp {
namespace mesh {

template<typename T>
class OptionalComponentsVector
{
	typedef typename T::ColorType ColorType;
	void enableColor(unsigned int size) {isColorEnabled = true; colors.resize(size);}
	ColorType& color(unsigned int i) {assert(isColorEnabled); return colors[i];}
	const ColorType& color (unsigned int i) const {assert(isColorEnabled); return colors[i];}

private:
	bool isColorEnabled = false;
	std::vector<ColorType> colors;
};

} // namespace mesh
} // namespace mgp

#endif // MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
