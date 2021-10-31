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

template<class Face, int N, typename T>
class OptionalFaceReferences :
		public OptionalFaceReferencesTriggerer<T>,
		public virtual OptionalInfo<T>
{
	template <typename, typename>
	friend class OptionalFaceReferencesVector;
private:
	// id 0 if use the array, 1 if we use the vector
	static const int VARIANT_ID = N >= 0 ? 0 : 1;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	typedef OptionalInfo<T> B;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>,
		typename std::vector<Face*>>::type;

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
