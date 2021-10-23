/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_COLOR_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_COLOR_VECTOR_H

#include <mgp/mesh/common/optional_color.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class OptionalColorVector
{
public:
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
};

template<typename T>
class OptionalColorVector<T, std::enable_if_t<common::hasOptionalColor<T>::value>> :
		private OptionalGenericVector<typename T::ColorType>
{
	typedef typename T::ColorType            ColorType;
	typedef OptionalGenericVector<ColorType> Base;

public:
	using Base::reserve;
	using Base::resize;
	bool             isColorEnabled() { return Base::isEnabled(); };
	void             enableColor(unsigned int size) { Base::enable(size); }
	void             disableColor() { Base::disable(); }
	ColorType&       color(unsigned int i) { return Base::at(i); }
	const ColorType& color(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::mesh

#endif // MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_COLOR_VECTOR_H
