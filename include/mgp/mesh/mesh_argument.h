/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ARGUMENT_H
#define MGP_MESH_ARGUMENT_H

#include "container/containers.h"

namespace mgp::mesh {

// to shorten triggerer for Vertex class
template<typename T>
using IfIsVertex = std::enable_if_t<std::is_base_of<VertexTriggerer, T>::value>;

// to shorten triggerer for Face class
template<typename T>
using IfIsFace = std::enable_if_t<std::is_base_of<FaceTriggerer, T>::value>;


/**
 * @brief The Argument fallback class, will be used when the template argument given to the
 * Mesh is not one of the known Elements: mgp::Vertex, mgp::Face, mgp::Edge...
 *
 * This class will just make the given template argument a Base of the Mesh, becoming a feature of
 * the Mesh.
 *
 * Example: if you want to add a label to your mesh, you just need to implement a small class having
 * a member string and getter and setter members, and use this class as template argument of the
 * mesh.
 */
template<typename T, typename = void>
class Argument : public T
{
};

template <typename T>
class Argument<T, IfIsVertex<T>>
{
};

template <typename T>
class Argument<T, IfIsFace<T>>
{
};

}

#endif // MGP_MESH_ARGUMENT_H
