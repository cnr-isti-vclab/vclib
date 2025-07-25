/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_ELEM_ALGORITHMS_BOUNDING_BOX_H
#define VCL_MESH_ELEM_ALGORITHMS_BOUNDING_BOX_H

#include <vclib/concepts.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief Compute the bounding box of a vertex
 *
 * Given a vertex `v`, this function computes and returns the bounding
 * box of the vertex. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam VertexType: The type of the vertex. It must satisfy the
 * VertexConcept.
 *
 * @param[in] v: The input vertex to compute the bounding box of
 * @return The bounding box of the input vertex
 *
 * @ingroup core_bounding_box
 */
template<VertexConcept VertexType>
auto boundingBox(const VertexType& v)
{
    return Box<typename VertexType::PositionType>(v.position());
}

/**
 * @brief Compute the bounding box of a vertex pointer
 *
 * Given a pointer to a vertex `v`, this function computes and returns the
 * bounding box of the vertex. The bounding box is represented by a `vcl::Box`
 * object.
 *
 * @tparam VertexType: The type of the vertex. It must satisfy the
 * VertexConcept.
 *
 * @param[in] v: A pointer to the input vertex to compute the bounding box of
 * @return The bounding box of the input vertex
 *
 * @ingroup core_bounding_box
 */
template<VertexConcept VertexType>
auto boundingBox(const VertexType* v)
{
    return Box<typename VertexType::PositionType>(v->position());
}

/**
 * @brief Compute the bounding box of a face
 *
 * Given a face `f`, this function computes and returns the bounding box
 * of the face. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam FaceType: The type of the face. It must satisfy the FaceConcept.
 *
 * @param[in] f The input face to compute the bounding box of
 * @return The bounding box of the input face
 *
 * @ingroup core_bounding_box
 */
template<FaceConcept FaceType>
auto boundingBox(const FaceType& f)
{
    using VertexType = FaceType::VertexType;

    Box<typename VertexType::PositionType> b;
    for (const VertexType* v : f.vertices())
        b.add(v->position());
    return b;
}

/**
 * @brief Compute the bounding box of a face pointer
 *
 * Given a pointer to a face `f`, this function computes and returns the
 * bounding box of the face. The bounding box is represented by a `vcl::Box`
 * object.
 *
 * @tparam FaceType: The type of the face. It must satisfy the FaceConcept.
 *
 * @param[in] f: A pointer to the input face to compute the bounding box of
 * @return The bounding box of the input face
 *
 * @ingroup core_bounding_box
 */
template<FaceConcept FaceType>
auto boundingBox(const FaceType* f)
{
    using VertexType = FaceType::VertexType;

    Box<typename VertexType::PositionType> b;
    for (const VertexType* v : f->vertices())
        b.add(v->position());
    return b;
}

/**
 * @brief Compute the bounding box of an edge
 *
 * Given an edge `e`, this function computes and returns the bounding box
 * of the edge. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam EdgeType: The type of the edge. It must satisfy the EdgeConcept.
 *
 * @param[in] e: The input edge to compute the bounding box of
 * @return The bounding box of the input edge
 *
 * @ingroup core_bounding_box
 */
template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType& e)
{
    using VertexType = EdgeType::VertexType;

    Box<typename VertexType::PositionType> b;
    for (const VertexType* v : e.vertices())
        b.add(v->position());
    return b;
}

/**
 * @brief Compute the bounding box of an edge pointer
 *
 * Given a pointer to an edge `e`, this function computes and returns the
 * bounding box of the edge. The bounding box is represented by a `vcl::Box`
 * object.
 *
 * @tparam EdgeType: The type of the edge. It must satisfy the EdgeConcept.
 * @param[in] e: A pointer to the input edge to compute the bounding box of
 *
 * @return The bounding box of the input edge
 *
 * @ingroup core_bounding_box
 */
template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType* e)
{
    using VertexType = EdgeType::VertexType;

    Box<typename VertexType::PositionType> b;
    for (const VertexType* v : e->vertices())
        b.add(v->position());
    return b;
}

} // namespace vcl

#endif // VCL_MESH_ELEM_ALGORITHMS_BOUNDING_BOX_H
