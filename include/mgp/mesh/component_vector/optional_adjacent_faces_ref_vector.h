/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H

#include <mgp/mesh/vertex/optional_adjacent_faces_ref.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class OptionalAdjacentFacesRefVector
{
public:
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
};

template<typename T>
class OptionalAdjacentFacesRefVector<T, std::enable_if_t<vert::hasOptionalAdjacenctFacesRef<T>()>> :
	private OptionalGenericVector<std::vector<typename T::FaceType*>>
{
	typedef typename T::FaceType                                      FaceType;
	typedef OptionalGenericVector<std::vector<typename T::FaceType*>> Base;

public:
	using Base::reserve;
	using Base::resize;
	bool                          isColorEnabled() const { return Base::isEnabled(); };
	void                          enableColor(unsigned int size) { Base::enable(size); }
	void                          disableColor() { Base::disable(); }
	std::vector<FaceType>&        adjFacesRefs(unsigned int i) { return Base::at(i); }
	const std::vector<FaceType>&  adjFacesRefs(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::mesh

#endif // MGP_MESH_COMPONENT_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H
