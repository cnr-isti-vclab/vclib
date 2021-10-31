/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H
#define MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <variant>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace mgp::mesh {
template <typename, typename>
class OptionalFaceReferencesVector;
}

namespace mgp::components {

template<typename T>
class OptionalFaceReferencesTriggerer
{
};

template<class Face, typename T>
class OptionalFaceReferences :
		public OptionalFaceReferencesTriggerer<T>,
		public virtual OptionalInfo<T>
{
	template <typename, typename>
	friend class OptionalFaceReferencesVector;
private:
	typedef OptionalInfo<T> B;

public:

private:
	unsigned int thisId() const { return ((T*) this)->id(); }
};

/**
 * Detector to check if a class has (inherits) OptionalOptionalFaceReferences
 */

template<typename T>
using hasOptionalFaceReferencesT = std::is_base_of<OptionalFaceReferencesTriggerer<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalFaceReferences=
	typename std::enable_if<hasOptionalFaceReferencesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalFaceReferences()
{
	return hasOptionalFaceReferencesT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H
