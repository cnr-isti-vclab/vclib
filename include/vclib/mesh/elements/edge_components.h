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

#ifndef VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/mark.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/scalar.h"
#include "../components/vertex_pointers.h"

/**
 * @defgroup edge_components Edge Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by the Edge element.
 *
 * All these components are listed inside the vcl::edge namespace.
 */

namespace vcl::edge {

/**
 * @addtogroup edge_components
 * @{
 */

/* Port AdjacentEdges class into edge namespace */
template<typename EdgeType>
using AdjacentEdges = comp::AdjacentEdges<EdgeType, -1>;

template<typename EdgeType>
using VerticalAdjacentEdges = comp::AdjacentEdges<EdgeType, -1, EdgeType>;

template<typename EdgeType>
using OptionalAdjacentEdges = comp::AdjacentEdges<EdgeType, -1, EdgeType, true>;

/* Port AdjacentFaces class into edge namespace */
template<typename FaceType>
using AdjacentFaces = comp::AdjacentFaces<FaceType, -1>;

template<typename FaceType, typename EdgeType>
using VerticalAdjacentFaces = comp::AdjacentFaces<FaceType, -1, EdgeType>;

template<typename FaceType, typename EdgeType>
using OptionalAdjacentFaces = comp::AdjacentFaces<FaceType, -1, EdgeType, true>;

/* Port BitFlags class into edge namespace */
using BitFlags = comp::BitFlags<>;

/* Port Color class into edge namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>; 

template<typename EdgeType>
using OptionalColor = comp::Color<EdgeType, true>;

/* Port CustomComponents class into edge namespace */
template<typename EdgeType>
using CustomComponents = comp::CustomComponents<EdgeType>;

/* Port Mark class into edge namespace */
using Mark = comp::Mark<>;

template<typename EdgeType>
using VerticalMark = comp::Mark<EdgeType>;

template<typename EdgeType>
using OptionalMark = comp::Mark<EdgeType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port Scalar class into edge namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf<>;
using Scalard = comp::Scalard<>;

template<typename ScalarType, typename EdgeType>
using VerticalScalar = comp::Scalar<ScalarType, EdgeType>;

template<typename EdgeType>
using VerticalScalarf = comp::Scalar<float, EdgeType>;

template<typename EdgeType>
using VerticalScalard = comp::Scalar<double, EdgeType>;

template<typename ScalarType, typename EdgeType>
using OptionalScalar = comp::Scalar<ScalarType, EdgeType, true>;

template<typename EdgeType>
using OptionalScalarf = comp::Scalar<float, EdgeType, true>;

template<typename EdgeType>
using OptionalScalard = comp::Scalar<double, EdgeType, true>;

/* Port VertexPointers class into edge namespace */
template<typename VertexType>
using VertexPointers = comp::VertexPointers<VertexType, 2>;

/** @} */ // end of edge_components group

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
