/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_CONTAIER_T_H
#define MGP_MESH_CONTAINER_CONTAIER_T_H

#include <vector>

#include <mgp/misc/type_traits.h>

namespace mgp::mesh {

/**
 * @brief The Container fallback class, will be used when the template argument given to the
 * Mesh is not one of the known Elements: mgp::Vertex, mgp::Face, mgp::Edge...
 *
 * This class will just make the given template argument a Base of the Mesh, becoming a feature of
 * the Mesh.
 *
 * Example: if you want to add a label to your mesh, you just need to implement a small class having
 * a member string and getter and setter members, and use this class as template argument of the
 * mesh.
 */
template<class T, typename = void>
class Container : public T
{
};

} // namespace mgp::mesh

#endif // MGP_MESH_CONTAINER_CONTAIER_T_H
