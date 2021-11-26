/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_ARGUMENT_H
#define VCL_MESH_ARGUMENT_H

#include "container/containers.h"

namespace vcl::mesh {

// to shorten triggerer for Vertex class
template<typename T>
using IfIsVertex = std::enable_if_t<std::is_base_of<VertexTriggerer, T>::value>;

// to shorten triggerer for Face class
template<typename T>
using IfIsFace = std::enable_if_t<std::is_base_of<FaceTriggerer, T>::value>;


/**
 * @brief The Argument fallback class, will be used when the template argument given to the
 * Mesh is not one of the known Elements: Vertex, Face, Edge...
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
class Argument<T, IfIsVertex<T>> : public VertexContainer<T>
{
};

template <typename T>
class Argument<T, IfIsFace<T>> : public FaceContainer<T>
{
};

}

#endif // VCL_MESH_ARGUMENT_H
