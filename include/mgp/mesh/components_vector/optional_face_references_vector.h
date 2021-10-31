/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H

#include <mgp/mesh/components_optional/optional_face_references.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class OptionalFaceReferencesVector
{
public:
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
};

template<typename T>
class OptionalFaceReferencesVector<
	T,
	std::enable_if_t<components::hasOptionalFaceReferences<T>()>> :
		// inherit from a generic vector of variants, that may contain:
		// - array of FaceType* (if the number of references is fixed)
		// - vector of FaceType* (if the number of references is dynamic)
		// which one will be used is managed by the OptionalFaceReferences class
		// this class is friend of OptionalFaceReferences, therefore it is allowed to use
		// T::FaceType::ARRAY_SIZE
		private OptionalGenericVector<std::variant<
			std::array<typename T::FaceType*, T::FaceType::ARRAY_SIZE>,
			std::vector<typename T::FaceType*>>>
{
private:
	using FaceType = typename T::FaceType;
	using Base = OptionalGenericVector<std::vector<typename T::FaceType*>>;
	using VariantContainer = std::variant<
		std::array<typename T::FaceType*, T::FaceType::ARRAY_SIZE>,
		std::vector<typename T::FaceType*>>;

public:
	using Base::reserve;
	using Base::resize;
	bool                    isFaceReferencesEnabled() const { return Base::isEnabled(); };
	void                    enableFaceReferences(unsigned int size) { Base::enable(size); }
	void                    disableFaceReferences() { Base::disable(); }
	VariantContainer&       faceRefs(unsigned int i) { return Base::at(i); }
	const VariantContainer& faceRefs(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::mesh

#endif // MGP_MESH_COMPONENT_VECTOR_OPTIONAL_ADJACENT_FACES_REF_VECTOR_H
