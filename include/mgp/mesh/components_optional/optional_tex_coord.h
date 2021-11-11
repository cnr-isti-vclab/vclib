/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_TEX_COORD_H
#define MGP_MESH_COMPONENTS_OPTIONAL_TEX_COORD_H

#include <mgp/space/tex_coord.h>

#include "optional_info.h"

namespace mgp::comp {

template<typename T>
class OptionalTexCoordTriggerer
{
};

template<typename Scalar, typename T>
class OptionalTexCoord : public OptionalTexCoordTriggerer<T>, public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using TexCoordType = mgp::TexCoord<Scalar>;

	const TexCoordType& texCoord() const { return B::contPtr->texCoord(thisId()); }
	TexCoordType&       texCoord() { return B::contPtr->texCoord(thisId()); }

private:
	unsigned int thisId() const { return ((T*) this)->id(); }
};

/**
 * Detector to check if a class has (inherits) OpionalNormal
 */

template<typename T>
using hasOptionalTexCoordT = std::is_base_of<OptionalTexCoordTriggerer<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalTexCoord = typename std::enable_if<hasOptionalTexCoordT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalTexCoord()
{
	return hasOptionalTexCoordT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_OPTIONAL_TEX_COORD_H
