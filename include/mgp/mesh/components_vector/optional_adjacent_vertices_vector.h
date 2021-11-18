/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_ADJACENT_VERTICES_VECTOR_H
#define MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_ADJACENT_VERTICES_VECTOR_H

#include <mgp/mesh/components_optional/optional_adjacent_vertices.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalAdjacentVerticesVector
{
public:
	void clear() {}
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalAdjacentVerticesVector<
	T,
	std::enable_if_t<comp::hasOptionalAdjacentVertices<T>()>> :
		private OptionalGenericVector<typename T::AdjVertsContainer>
{
private:
	using AdjVertsContainer = typename T::AdjVertsContainer;
	using Base = OptionalGenericVector<AdjVertsContainer>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool             isAdjacentVerticesEnabled() const { return Base::isEnabled(); };
	void             enableAdjacentVertices(unsigned int size) { Base::enable(size); }
	void             disableAdjacentVertices() { Base::disable(); }
	AdjVertsContainer&       adjVerts(unsigned int i) { return Base::at(i); }
	const AdjVertsContainer& adjVerts(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_ADJACENT_VERTICES_VECTOR_H
