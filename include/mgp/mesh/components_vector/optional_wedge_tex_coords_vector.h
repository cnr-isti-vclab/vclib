/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_WEDGE_TEX_COORDS_VECTOR_H
#define MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_WEDGE_TEX_COORDS_VECTOR_H

#include <mgp/mesh/components_optional/optional_wedge_tex_coords.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalWedgeTexCoordsVector
{
public:
	void clear() {}
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalWedgeTexCoordsVector<T, std::enable_if_t<comp::hasOptionalWedgeTexCoords<T>()>> :
		private OptionalGenericVector<typename T::WedgeTexCoordsContainer>
{
	using WedgeTexCoordsContainer = typename T::WedgeTexCoordsContainer;
	using Base                    = OptionalGenericVector<WedgeTexCoordsContainer>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;
	bool                isWedgeTexCoordsEnabled() const { return Base::isEnabled(); };
	void                enableWedgeTexCoords(unsigned int size) { Base::enable(size); }
	void                disableWedgeTexCoords() { Base::disable(); }
	WedgeTexCoordsContainer&       wedgeTexCoords(unsigned int i) { return Base::at(i); }
	const WedgeTexCoordsContainer& wedgeTexCoords(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_WEDGE_TEX_COORDS_VECTOR_H
