/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_OPTIONAL_COLOR_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_OPTIONAL_COLOR_VECTOR_H

#include <mgp/mesh/components_optional/optional_color.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalColorVector
{
public:
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalColorVector<T, std::enable_if_t<components::hasOptionalColor<T>()>> :
		private OptionalGenericVector<typename T::ColorType>
{
	using ColorType = typename T::ColorType;
	using Base = OptionalGenericVector<ColorType>;

public:
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool             isColorEnabled() const { return Base::isEnabled(); };
	void             enableColor(unsigned int size) { Base::enable(size); }
	void             disableColor() { Base::disable(); }
	ColorType&       color(unsigned int i) { return Base::at(i); }
	const ColorType& color(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENT_VECTOR_OPTIONAL_COLOR_VECTOR_H
