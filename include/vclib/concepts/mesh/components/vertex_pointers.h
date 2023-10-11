/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_POINTERS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_POINTERS_H

#include "component.h"

#include <vector>

namespace vcl::comp {

/**
 * @brief HasVertexPointers concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to an VertexPointers component of
 * a given element.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasVertexPointers = requires(
    T o,
    const T& co,
    typename T::VertexType v,
    std::vector<typename T::VertexType*> vec)
{
    T::VERTEX_NUMBER;
    typename T::VertexType;
    typename T::VertexIterator;
    typename T::ConstVertexIterator;

    { co.vertexNumber() } -> std::same_as<uint>;
    { o.vertex(uint()) } -> std::same_as<typename T::VertexType*&>;
    { co.vertex(uint()) } -> std::same_as<const typename T::VertexType*>;
    { o.vertexMod(int()) } -> std::same_as<typename T::VertexType*&>;
    { co.vertexMod(int()) } -> std::same_as<const typename T::VertexType*>;

    { o.setVertex(&v, uint()) } -> std::same_as<void>;
    { o.setVertices(vec) } -> std::same_as<void>;

    { co.containsVertex(&v) } -> std::same_as<bool>;
    { o.findVertex(&v) } -> std::same_as<typename T::VertexIterator>;
    { co.findVertex(&v) } -> std::same_as<typename T::ConstVertexIterator>;

    { co.indexOfVertex(&v) } -> std::same_as<uint>;
    { co.indexOfEdge(&v, &v) } -> std::same_as<uint>;

    { o.vertexBegin() } -> std::same_as<typename T::VertexIterator>;
    { o.vertexEnd() } -> std::same_as<typename T::VertexIterator>;
    { co.vertexBegin() } -> std::same_as<typename T::ConstVertexIterator>;
    { co.vertexEnd() } -> std::same_as<typename T::ConstVertexIterator>;
    o.vertices();
    co.vertices();
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_VERTEX_POINTERS_H
