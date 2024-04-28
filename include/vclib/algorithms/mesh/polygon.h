/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_ALGORITHMS_MESH_POLYGON_H
#define VCL_ALGORITHMS_MESH_POLYGON_H

#include <vclib/algorithms/core/polygon/topology.h>

namespace vcl {

/**
 * @brief Given a vector of indices of vertices in a mesh representing a polyon,
 * this function adds N triangular faces to the mesh, that are the triangulation
 * of the input polygon. Triangle edges that are internal in the polygon are
 * marked as faux.
 *
 * This function assumes that the first (triangular) face has been already added
 * to the mesh and just needs to be filled with vertex references. This is
 * useful in some cases (e.g. when reading from file and you realize just at
 * some point that you need to manage a polygon). In all the other cases, you
 * should use the function that does not take a face in input, that is:
 *
 * @code{.cpp}
 * uint fid = addTriangleFacesFromPolygon(mesh, polygon);
 * @endcode
 *
 * @throws BadVertexIndexException if there is a vertex index in the polygon
 * that is out of range, or that references a vertex that results deleted.
 *
 * @tparam MeshType: the type of the face that satisfies the FaceMeshConcept.
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in,out] m: the mesh on which add the triangulation of the polygon.
 * @param[in,out] f: the first face of the triangulation, that will be filled.
 * @param[in] polygon: the vertex indices in the mesh representing the polygon.
 */
template<FaceMeshConcept MeshType, FaceConcept FaceType>
void addTriangleFacesFromPolygon(
    MeshType&                m,
    FaceType&                f,
    const std::vector<uint>& polygon)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;

    // from the ids, create a polygon of coordinates
    std::vector<CoordType> polCoords(polygon.size());
    for (uint i = 0; i < polygon.size(); ++i) {
        if (polygon[i] >= m.vertexContainerSize()) {
            throw BadVertexIndexException(
                "Index " + std::to_string(polygon[i]) +
                " is out of range in Vertex Container.");
        }
        if (m.vertex(polygon[i]).deleted()) {
            throw BadVertexIndexException(
                "Vertex " + std::to_string(polygon[i]) + " is deleted.");
        }
        polCoords[i] = m.vertex(polygon[i]).coord();
    }

    // compute earcut of the polygons
    std::vector<uint> tris =
        Polygon<CoordType>::earCut(polCoords.begin(), polCoords.end());

    // faux edges management: create a set of unordered edges of the polygon
    // note: we use indices from 0 to polygon.size() because that are the output
    // indices given by the earcut algorithm
    std::set<std::pair<uint, uint>, UnorderedPairComparator<uint>>
        unorderedEdges;
    for (uint i = 0; i < polygon.size(); ++i)
        unorderedEdges.emplace(i, (i + 1) % (uint) polygon.size());

    if constexpr (FaceType::VERTEX_NUMBER < 0) {
        f.resizeVertices(3);
    }

    // set the first triangle of the loaded polygon
    for (uint i = 0; i < f.vertexNumber(); ++i) {
        f.setVertex(i, polygon[tris[i]]);
    }

    if constexpr (face::HasFaceBitFlags<FaceType>) {
        if (unorderedEdges.find(std::make_pair(tris[0], tris[1])) ==
            unorderedEdges.end())
            f.edgeFaux(0) = true;
        if (unorderedEdges.find(std::make_pair(tris[1], tris[2])) ==
            unorderedEdges.end())
            f.edgeFaux(1) = true;
        if (unorderedEdges.find(std::make_pair(tris[2], tris[0])) ==
            unorderedEdges.end())
            f.edgeFaux(2) = true;
    }

    // remaining triangles, need to create more faces in the mesh
    for (uint i = 3; i < tris.size(); i += 3) {
        uint ff = m.addFace();

        if constexpr (FaceType::VERTEX_NUMBER < 0) {
            m.face(ff).resizeVertices(3);
        }

        for (uint j = 0; j < m.face(ff).vertexNumber(); ++j) {
            m.face(ff).setVertex(j, polygon[tris[i + j]]);
        }

        if constexpr (face::HasFaceBitFlags<FaceType>) {
            if (unorderedEdges.find(std::make_pair(tris[i], tris[i + 1])) ==
                unorderedEdges.end())
                m.face(ff).edgeFaux(0) = true;
            if (unorderedEdges.find(std::make_pair(tris[i + 1], tris[i + 2])) ==
                unorderedEdges.end())
                m.face(ff).edgeFaux(1) = true;
            if (unorderedEdges.find(std::make_pair(tris[i + 2], tris[i + 0])) ==
                unorderedEdges.end())
                m.face(ff).edgeFaux(2) = true;
        }
    }
}

/**
 * @brief Given a vector of indices of vertices in a mesh representing a polyon,
 * this function adds N triangular faces to the mesh, that are the triangulation
 * of the input polygon. Triangle edges that are internal in the polygon are
 * marked as faux. This function returns the index of the first added triangle.
 *
 * @throws BadVertexIndexException if there is a vertex index in the polygon
 * that is out of range, or that references a vertex that results deleted.
 *
 * @tparam MeshType: the type of the face that satisfies the FaceMeshConcept.
 *
 * @param[in,out] m: the mesh on which add the triangulation of the polygon.
 * @param[in] polygon: the vertex indices in the mesh representing the polygon.
 * @return The index of the first triangle added to the mesh.
 */
template<FaceMeshConcept MeshType>
uint addTriangleFacesFromPolygon(MeshType& m, const std::vector<uint>& polygon)
{
    uint fid = m.addFace();
    addTriangleFacesFromPolygon(m, m.face(fid), polygon);
    return fid;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_POLYGON_H
