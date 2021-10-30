/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_COLOR_H
#define MGP_MESH_COMPONENTS_COLOR_H

#include <mgp/space/color.h>

namespace mgp::components {

/**
 * @brief The Color class represents a RGBA color that will be part of an Element
 * (e.g. Vertex, Face, ...).
 *
 * Allows to get and set an object of type mgp::Color.
 */
class Color
{
public:
	typedef mgp::Color ColorType;

	/**
	 * @brief get a const reference of the color of the element.
	 * @return
	 */
	const mgp::Color& color() const { return c; }

	/**
	 * @brief get a reference pf the color of the element.
	 * @return
	 */
	mgp::Color& color() { return c; }

private:
	mgp::Color c;
};

/**
 * Detector to check if a class has (inherits) Color
 */

template<typename T>
using hasColorT = std::is_base_of<Color, T>;

template<typename T>
constexpr bool hasColor()
{
	return hasColorT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_COLOR_H
