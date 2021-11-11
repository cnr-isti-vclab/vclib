/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_TEX_COORD_H
#define MGP_MESH_COMPONENTS_TEX_COORD_H

#include <mgp/space/tex_coord.h>

namespace mgp::comp {

class TexCoordTriggerer
{
};

template<class Scalar>
class TexCoord : public TexCoordTriggerer
{
public:
	using TexCoordType = mgp::TexCoord<Scalar>;

	const TexCoordType& texCoord() const { return t; }
	TexCoordType        texCoord() { return t; }

private:
	mgp::TexCoord<Scalar> t;
};

using TexCoordf = TexCoord<float>;
using TexCoordd = TexCoord<double>;

/**
 * Detector to check if a class has (inherits) TexCoord
 */

template<typename T>
using hasTexCoordT = std::is_base_of<TexCoordTriggerer, T>;

template<typename T>
bool constexpr hasTexCoord()
{
	return hasTexCoordT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_TEXCOORD_H
