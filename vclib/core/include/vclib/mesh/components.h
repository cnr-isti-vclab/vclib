// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_H
#define VCL_MESH_COMPONENTS_H

#include "components/adjacent_edges.h"
#include "components/adjacent_faces.h"
#include "components/adjacent_vertices.h"
#include "components/bit_flags.h"
#include "components/color.h"
#include "components/custom_components.h"
#include "components/material_index.h"
#include "components/normal.h"
#include "components/polygon_bit_flags.h"
#include "components/position.h"
#include "components/principal_curvature.h"
#include "components/quality.h"
#include "components/tangent.h"
#include "components/tex_coord.h"
#include "components/triangle_bit_flags.h"
#include "components/vertex_references.h"
#include "components/wedges.h"

/**
 * @defgroup components Components
 * @ingroup mesh
 *
 * @brief List of all the Component classes, along with their concepts and
 * functions.
 *
 * @section components_indices_sec Global Element Indices vs. Local Container
 * Positions
 *
 * In VCLib, there is a fundamental architectural distinction between two
 * different index spaces:
 *
 * 1. **Global Element Index (`IndexType`) - Positions in `ElementContainer`**:
 *    - Represents the global identity or location of an element (Vertex, Face,
 *      Edge) within the mesh's corresponding `ElementContainer` (e.g.
 *      `m.vertex(idx)`).
 *    - Typed as `IndexType` (e.g. `typename MeshType::IndexType`), which can be
 *      configured as 32-bit (`uint32_t`) for standard meshes or 64-bit
 *      (`uint64_t`) for large-scale datasets.
 *    - Its invalid/unassigned sentinel value is `vcl::NULL_INDEX<IndexType>`
 *      (evaluating to `(IndexType)-1`).
 *    - Returned by element-query methods such as `face.vertexIndex(uint i)`,
 *      `face.adjFaceIndex(uint i)`, `edge.adjEdgeIndex(uint i)`, and
 *      `wedge.vertexIndex()`.
 *    - Stored inside reference containers (e.g. `VertexReferences`,
 *      `AdjacentFaces`, `AdjacentEdges`, `AdjacentVertices`).
 *    - Passed to search or membership queries such as
 *      `face.indexOfVertex(IndexType vi)` or `face.containsVertex(IndexType
 * vi)`.
 *
 * 2. **Local Container Position (`uint`) - Offsets in `ContainerComponent`**:
 *    - Represents the 0-based offset or slot within a single element's local
 *      `ContainerComponent` (such as `VertexReferences`, `AdjacentFaces`,
 *      `AdjacentEdges`, `AdjacentVertices`, and per-corner `Wedges`).
 *    - Always typed as `uint` (`uint32_t`), because the number of sub-elements
 *      or references per element (e.g. 3 vertices of a triangle, $k$ vertices
 *      of a polygon, or the valence of a vertex) is a small, local cardinality
 *      that never requires 64 bits.
 *    - Its "not found" sentinel value is `vcl::UINT_NULL` (`0xFFFFFFFF`).
 *    - Passed as the position argument `uint i` to access local items (e.g.
 *      `face.vertex(uint i)`, `face.vertexIndex(uint i)`, or `face.wedge(uint
 *      i)`).
 *    - Returned by search methods such as `face.indexOfVertex(IndexType vi)`,
 *      `face.indexOfEdge(IndexType vi1, IndexType vi2)`, or
 *      `face.indexOfAdjFace(IndexType fi)`, which locate the local position of
 *      a global element within the local container.
 *    - Returned by `wedge.index()`, representing the corner index (0, 1, 2...)
 *      within the face.
 *
 * @note **Crucial Sentinel Distinction**:
 * Always use `NULL_INDEX` when comparing global element references
 * (`IndexType`), and `UINT_NULL` when checking local container positions
 * (`uint`). On 64-bit meshes, confusing `UINT_NULL` with `NULL_INDEX` leads to
 * silent comparison bugs because `0xFFFFFFFF != 0xFFFFFFFFFFFFFFFF`.
 */

/**
 * @defgroup components_concepts Components Concepts
 * @ingroup mesh_concepts
 * @ingroup components
 *
 * @brief List of concepts for types related to the Component classes of the
 * library.
 */

#endif // VCL_MESH_COMPONENTS_H
