/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_TEX_COORD_VECTOR_H
#define MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_TEX_COORD_VECTOR_H

#include <mgp/mesh/properties_optional/optional_tex_coord.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalTexCoordVector
{
public:
	void clear() {}
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalTexCoordVector<T, std::enable_if_t<prop::hasOptionalTexCoord<T>()>> :
		private OptionalGenericVector<typename T::TexCoordType>
{
	using TexCoordType = typename T::TexCoordType;
	using Base         = OptionalGenericVector<TexCoordType>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;
	bool                isTexCoordEnabled() const { return Base::isEnabled(); };
	void                enableTexCoord(unsigned int size) { Base::enable(size); }
	void                disableTexCoord() { Base::disable(); }
	TexCoordType&       texCoord(unsigned int i) { return Base::at(i); }
	const TexCoordType& texCoord(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_PROPERTIES_VECTOR_OPTIONAL_TEX_COORD_VECTOR_H
