/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H
#define MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H

#include <mgp/mesh/components_optional/optional_face_references.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

template<typename, typename = void>
class OptionalFaceReferencesVector
{
public:
	void clear() {}
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalFaceReferencesVector<
	T,
	std::enable_if_t<comp::hasOptionalFaceReferences<T>()>> :
		private OptionalGenericVector<typename T::FaceRefsContainer>
{
private:
	using FaceRefsContainer = typename T::FaceRefsContainer;
	using Base = OptionalGenericVector<FaceRefsContainer>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool             isFaceReferencesEnabled() const { return Base::isEnabled(); };
	void             enableFaceReferences(unsigned int size) { Base::enable(size); }
	void             disableFaceReferences() { Base::disable(); }
	FaceRefsContainer&       faceRefs(unsigned int i) { return Base::at(i); }
	const FaceRefsContainer& faceRefs(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::internal

#endif // MGP_MESH_COMPONENTS_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H
