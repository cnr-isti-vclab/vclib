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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H

#include <vclib/algorithms/mesh/import_export/append_replace_to_buffer.h>
#include <vclib/algorithms/mesh/import_export/export_buffer.h>
#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/mesh/requirements.h>
#include <vclib/render/drawable/mesh/mesh_render_info.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>

namespace vcl {

/**
 * @brief The MeshRenderData class provides a common interface to automatically
 * update the buffers used to render a mesh, with the possibility to update only
 * a subset of the buffers, taking into account different scenarios (e.g.,
 * vertex duplication, polygonal faces triangulation) consistently.
 *
 * The class uses the Curiously Recurring Template Pattern (CRTP) to allow this
 * class to call functions to set the buffers (that are managed by the derived
 * class, since they depend on the rendering backend used) when the users
 * requires to update the mesh data (that may be all the data, or a subset).
 *
 * It also provides a set of protected functions that may be used by the derived
 * class to fill the buffers in a consistent way, managing automatically all the
 * possible scenarios.
 *
 * The derived class should implement all the functions that set the buffers
 * (the `set*(const MeshType& mesh)` member functions listed in this class). If
 * the derived class does not implement one of these functions, the base class
 * will use the default implementation, that does nothing.
 *
 * An example of implementation in a derived class is the following (assuming
 * that `Base` is this class, and `MeshType` is the mesh type that will be used
 * to render the mesh). Here we first fill the vertex coordinates to a
 * std::vector:
 *
 * @code{.cpp}
 * void setVertexCoordsBuffer(const MeshType& mesh)
 * {
 *     // get the number of vertices (with eventual duplication)
 *     uint nv = Base::numVerts();
 *
 *     std::vector<float> vertexCoords(nv * 3);
 *     // fill the vertex coordinates
 *     Base::fillVertexCoords(mesh, vertexCoords.data());
 *
 *     // create the gpu vertex buffer using the desired rendering backend,
 *     // (be sure to first delete the previous buffer if it exists) and send
 *     // the data to the gpu
 * }
 * @endcode
 *
 * Refer to the documentation of the functions that set the buffers for more
 * details on how to fill the buffers.
 */
template<typename MeshRenderDerived>
class MeshRenderData
{
    using MRI = MeshRenderInfo;

    // Auxiliary data that can be used by the derived class to properly allocate
    // and fill the buffers

    uint mNumVerts       = 0;
    uint mNumTris        = 0;
    uint mNumEdges       = 0;
    uint nWireframeLines = 0;

    // Vector that tells, for each non-duplicated vertex, which wedges it
    // belongs to. Each pair is the face index and the vertex index in the face.
    // It allows to access the wedge texcoords for each non-duplicated vertex
    std::vector<std::pair<uint, uint>> mVertWedgeMap;

    // The list of vertices that has been duplicated (each element of the list
    // is the index of the vertex to duplicate)
    std::list<uint> mVertsToDuplicate;

    // A list that tells, for each duplicated vertex, the list of faces that
    // must be reassigned to the corresponding duplicated vertex.
    // Each duplicated vertex has a list of pairs face/vertex index in the face,
    // that must be/have been reassigned to the duplicated vertex
    std::list<std::list<std::pair<uint, uint>>> mFacesToReassign;

    // data used to manage the mapping beteween the original polygonal faces
    // and the triangle faces

    // map that stores the correspondence between the original polygonal faces
    // and the triangle faces
    TriPolyIndexBiMap mIndexMap;

    // bitset that tells which buffers must be filled (this value has been set
    // at construction time). It may differ from the value passed to the update
    // function, since the user may want to update only a subset of the buffers
    MRI::BuffersBitSet mBuffersToFill = MRI::BUFFERS_ALL;

public:
    /**
     * @brief Update the buffers used to render the mesh.
     *
     * @param[in] mesh: the input mesh from which to get the data
     * @param[in] buffersToUpdate: the buffers that must be updated. By default,
     * all the buffers are updated.
     */
    void update(
        const MeshConcept auto& mesh,
        MRI::BuffersBitSet      buffersToUpdate = MRI::BUFFERS_ALL)
    {
        using enum MRI::Buffers;

        MRI::BuffersBitSet btu = mBuffersToFill & buffersToUpdate;

        // first thing to do
        updateAuxiliaryData(mesh, btu);

        // set data for vertices
        updateVerticesData(mesh, btu);

        // set data for faces
        updateFacesData(mesh, btu);

        // set data for edges
        updateEdgesData(mesh, btu);

        // set data for mesh
        updateMeshData(mesh, btu);

        // set data for textures
        updateTextureData(mesh, btu);
    }

protected:
    MeshRenderData() = default;

    MeshRenderData(MRI::BuffersBitSet buffersToFill) :
            mBuffersToFill(buffersToFill)
    {
    }

    void swap(MeshRenderData& other)
    {
        using std::swap;
        swap(mNumVerts, other.mNumVerts);
        swap(mNumTris, other.mNumTris);
        swap(mVertWedgeMap, other.mVertWedgeMap);
        swap(mVertsToDuplicate, other.mVertsToDuplicate);
        swap(mFacesToReassign, other.mFacesToReassign);
        swap(mIndexMap, other.mIndexMap);
        swap(mBuffersToFill, other.mBuffersToFill);
    }

    /**
     * @brief Returns the number of vertices that will be used to render the
     * mesh.
     *
     * The number of vertices must be used to compute the size of the buffers
     * that will store the vertex data (coordinates, normals, colors, etc).
     *
     * It can be used along with the functions `fillVertex*` provided by this
     * class. A common workflow is the following:
     *
     * @code{.cpp}
     * uint nv = numVerts();
     * // assuming that the buffer is a vector of floats
     * std::vector<float> vertexCoords(nv * 3);
     * fillVertexCoords(mesh, vertexCoords.data());
     * @endcode
     *
     * @note The returned values may be different from the number of vertices
     * in the input mesh. This is because the mesh may have duplicated vertices
     * (e.g., when the mesh has wedge texture coordinates).
     *
     * @note Always check the required buffer size before filling the buffers
     * on the `fill*` functions documentation.
     *
     * @return The number of vertices that will be used to render the mesh.
     */
    uint numVerts() const { return mNumVerts; }

    /**
     * @brief Returns the number of triangles that will be used to render the
     * mesh.
     *
     * The number of triangles must be used to compute the size of the buffers
     * that will store the triangles data (indices, normals, colors, etc).
     *
     * It can be used along with the functions `fillTriangle*` provided by this
     * class. A common workflow is the following:
     *
     * @code{.cpp}
     * uint nt = numTris();
     * // assuming that the buffer is a vector of uints
     * std::vector<uint> triIndices(nt * 3);
     * fillTriangleIndices(mesh, triIndices.data());
     * @endcode
     *
     * @note The returned values may be different from the number of faces
     * in the input mesh. This is because the mesh may have polygonal faces
     * that are triangulated.
     *
     * @note Always check the required buffer size before filling the buffers
     * on the `fill*` functions documentation.
     *
     * @return The number of triangles that will be used to render the mesh.
     */
    uint numTris() const { return mNumTris; }

    /**
     * @brief Returns the number of edges that will be used to render the
     * mesh.
     *
     * The number of edges must be used to compute the size of the buffers
     * that will store the edges data (indices, normals, colors, etc).
     *
     * It can be used along with the functions `fillEdge*` provided by this
     * class. A common workflow is the following:
     *
     * @code{.cpp}
     * uint ne = numEdges();
     * // assuming that the buffer is a vector of uints
     * std::vector<uint> edgeIndices(ne * 2);
     * fillEdgeIndices(mesh, edgeIndices.data());
     * @endcode
     *
     * @note Always check the required buffer size before filling the buffers
     * on the `fill*` functions documentation.
     *
     * @return The number of edges that will be used to render the mesh.
     */
    uint numEdges() const { return mNumEdges; }

    /**
     * @brief Returns the number of wireframe lines that will be used to render
     * the mesh.
     *
     * The number of wireframe lines must be used to compute the size of the
     * buffers that will store the wireframe data (indices).
     *
     * It can be used along with the functions `fillWireframe*` provided by this
     * class. A common workflow is the following:
     *
     * @code{.cpp}
     * uint nw = numWireframeLines();
     * // assuming that the buffer is a vector of uints
     * std::vector<uint> wireframeIndices(nw * 2);
     * fillWireframeIndices(mesh, wireframeIndices.data());
     * @endcode
     *
     * @note The returned values may be different from the number of faces * 3
     * in the input mesh. This is because the mesh may have polygonal faces
     * that are triangulated.
     *
     * @note Always check the required buffer size before filling the buffers
     * on the `fill*` functions documentation.
     *
     * @return The number of wireframe lines that will be used to render the
     * mesh.
     */
    uint numWireframeLines() const { return nWireframeLines; }

    // utility functions to fill the buffers

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the vertex coordinates of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numVerts() * 3`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillVertexCoords(const MeshConcept auto& mesh, auto* buffer)
    {
        vertexCoordsToBuffer(mesh, buffer);
        appendDuplicateVertexCoordsToBuffer(mesh, mVertsToDuplicate, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the vertex normals of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numVerts() * 3`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillVertexNormals(const MeshConcept auto& mesh, auto* buffer)
    {
        vertexNormalsToBuffer(mesh, buffer);
        appendDuplicateVertexNormalsToBuffer(mesh, mVertsToDuplicate, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the vertex colors of the mesh (each color is packed in a single uint).
     *
     * The buffer must be preallocated with the correct size: `numVerts()`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillVertexColors(
        const MeshConcept auto& mesh,
        auto*                   buffer,
        Color::Format           fmt)
    {
        vertexColorsToBuffer(mesh, buffer, fmt);
        appendDuplicateVertexColorsToBuffer(
            mesh, mVertsToDuplicate, buffer, fmt);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the vertex texcoords of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numVerts() * 2`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillVertexTexCoords(const MeshConcept auto& mesh, auto* buffer)
    {
        vertexTexCoordsToBuffer(mesh, buffer);
        appendDuplicateVertexTexCoordsToBuffer(mesh, mVertsToDuplicate, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the wedge texcoors of the mesh.
     *
     * Although the wedge texcoords are associated to the faces in the vclib
     * meshes, for rendering purposes it is useful to have them associated to
     * the vertices (that must be duplicated accordingly, and only when
     * necessary).
     *
     * The buffer must be preallocated with the correct size: `numVerts() * 2`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillWedgeTexCoords(const FaceMeshConcept auto& mesh, auto* buffer)
    {
        wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
            mesh, mVertWedgeMap, mFacesToReassign, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the triangle indices of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numTris() * 3`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillTriangleIndices(const FaceMeshConcept auto& mesh, auto* buffer)
    {
        triangulatedFaceIndicesToBuffer(
            mesh, buffer, mIndexMap, MatrixStorageType::ROW_MAJOR, mNumTris);
        replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
            mesh, mVertsToDuplicate, mFacesToReassign, mIndexMap, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the triangle normals of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numTris() * 3`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillTriangleNormals(const FaceMeshConcept auto& mesh, auto* buffer)
    {
        triangulatedFaceNormalsToBuffer(
            mesh, buffer, mIndexMap, MatrixStorageType::ROW_MAJOR);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the triangle colors of the mesh (each color is packed in a single uint).
     *
     * The buffer must be preallocated with the correct size: `numTris()`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillTriangleColors(
        const FaceMeshConcept auto& mesh,
        auto*                       buffer,
        Color::Format               fmt)
    {
        triangulatedFaceColorsToBuffer(mesh, buffer, mIndexMap, fmt);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the vertex texture indices of the mesh.
     *
     * Although the vertex texcoords are associated to the vertices in the vclib
     * meshes, for rendering purposes the index of each vertex texcoord is
     * associated to the triangles (that must be triangulated accordingly).
     *
     * The buffer must be preallocated with the correct size: `numTris()`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillVertexTextureIndices(
        const FaceMeshConcept auto& mesh,
        auto*                       buffer)
    {
        vertexTexCoordIndicesAsTriangulatedFaceTexCoordIndicesToBuffer(
            mesh, buffer, mIndexMap);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the wedge texture indices of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numTris()`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillWedgeTextureIndices(const FaceMeshConcept auto& mesh, auto* buffer)
    {
        triangulatedFaceWedgeTexCoordIndicesToBuffer(mesh, buffer, mIndexMap);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the edge indices of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numEdges() * 2`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillEdgeIndices(const EdgeMeshConcept auto& mesh, auto* buffer)
    {
        edgeIndicesToBuffer(mesh, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the edge normals of the mesh.
     *
     * The buffer must be preallocated with the correct size: `numEdges() * 3`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillEdgeNormals(const EdgeMeshConcept auto& mesh, auto* buffer)
    {
        edgeNormalsToBuffer(mesh, buffer);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the edge colors of the mesh (each color is packed in a single uint).
     *
     * The buffer must be preallocated with the correct size: `numEdges()`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillEdgeColors(
        const EdgeMeshConcept auto& mesh,
        auto*                       buffer,
        Color::Format               fmt)
    {
        edgeColorsToBuffer(mesh, buffer, fmt);
    }

    /**
     * @brief Given the mesh and a pointer to a buffer, fills the buffer with
     * the wireframe indices of the mesh.
     *
     * The buffer must be preallocated with the correct size:
     * `numWireframeLines()`.
     *
     * @param[in] mesh: the input mesh
     * @param[out] buffer: the buffer to fill
     */
    void fillWireframeIndices(const FaceMeshConcept auto& mesh, auto* buffer)
    {
        wireframeIndicesToBuffer(mesh, buffer);
    }

    // functions that must be may implemented by the derived classes to set
    // the buffers:

    /**
     * @brief Function that sets the content of vertex coordinates buffer and
     * sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the vertex
     * coordinates using the `numVerts() * 3` and `fillVertexCoords()`
     * functions, and then send the data to the GPU using the rendering backend.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexCoordsBuffer(const MeshConcept auto&) {}

    /**
     * @brief Function that sets the content of vertex normals buffer and sends
     * the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the vertex
     * normals using the `numVerts() * 3` and `fillVertexNormals()` functions,
     * and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-vertex normals
     * since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexNormalsBuffer(const MeshConcept auto&) {}

    /**
     * @brief Function that sets the content of vertex colors buffer and sends
     * the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the vertex
     * colors using the `numVerts()` and `fillVertexColors()` functions, and
     * then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-vertex colors
     * since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexColorsBuffer(const MeshConcept auto&) {}

    /**
     * @brief Function that sets the content of vertex texture coordinates
     * buffer and sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the vertex
     * texcoords using the `numVerts() * 2` and `fillVertexTexCoords()`
     * functions, and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-vertex
     * texcoords since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexTexCoordsBuffer(const MeshConcept auto&) {}

    /**
     * @brief Function that sets the content of wedge texture coordinates buffer
     * and sends the data to the GPU.
     *
     * Although the wedge texcoords are associated to the faces in the vclib
     * meshes, for rendering purposes it is useful to have them associated to
     * the vertices (that must be duplicated accordingly, and only when
     * necessary).
     *
     * The function should allocate and fill a cpu buffer to store the wedge
     * texcoords using the `numVerts() * 2` and `fillWedgeTexCoords()`
     * functions, and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-face wedge
     * texcoords since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setWedgeTexCoordsBuffer(const MeshConcept auto&) {}

    /**
     * @brief Function that sets the content of triangle indices buffer and
     * sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the triangle
     * indices using the `numTris() * 3` and `fillTriangleIndices()` functions,
     * and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide faces since the
     * function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setTriangleIndicesBuffer(const FaceMeshConcept auto&) {};

    /**
     * @brief Function that sets the content of triangle normals buffer and
     * sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the triangle
     * normals using the `numTris() * 3` and `fillTriangleNormals()` functions,
     * and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-face normals
     * since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setTriangleNormalsBuffer(const FaceMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of triangle colors buffer and
     * sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the triangle
     * colors using the `numTris()` and `fillTriangleColors()` functions, and
     * then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-face colors
     * since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setTriangleColorsBuffer(const FaceMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of vertex texture indices buffer
     * and sends the data to the GPU.
     *
     * Although the vertex texcoords are associated to the vertices in the vclib
     * meshes, for rendering purposes the index of each vertex texcoord is
     * associated to the triangles (that must be triangulated accordingly).
     *
     * The function should allocate and fill a cpu buffer to store the vertex
     * texcoord indices using the `numTris()` and `fillVertexTextureIndices()`
     * functions, and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-vertex
     * texcoords since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexTextureIndicesBuffer(const FaceMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of wedge texture indices buffer and
     * sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the wedge
     * texcoord indices using the `numTris()` and `fillWedgeTextureIndices()`
     * functions, and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-face wedge
     * texcoords since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setWedgeTextureIndicesBuffer(const FaceMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of wireframe indices buffer and
     * sends the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the wireframe
     * indices using the `numWireframeLines() * 2` and `fillWireframeIndices()`
     * functions, and then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide faces since the
     * function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setWireframeIndicesBuffer(const FaceMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of edge indices buffer and sends
     * the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the edge
     * indices using the `numEdges() * 2` and `fillEdgeIndices()` functions, and
     * then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide edges since the
     * function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setEdgeIndicesBuffer(const EdgeMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of edge normals buffer and sends
     * the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the edge
     * normals using the `numEdges() * 3` and `fillEdgeNormals()` functions, and
     * then send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-edge normals
     * since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setEdgeNormalsBuffer(const EdgeMeshConcept auto&) {}

    /**
     * @brief Function that sets the content of edge colors buffer and sends
     * the data to the GPU.
     *
     * The function should allocate and fill a cpu buffer to store the edge
     * colors using the `numEdges()` and `fillEdgeColors()` functions, and then
     * send the data to the GPU using the rendering backend.
     *
     * There is no need to check whether the Mesh can provide per-edge colors
     * since the function is called only if the mesh has them.
     *
     * See the @ref MeshRenderData class documentation for an example of
     * implementation.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setEdgeColorsBuffer(const EdgeMeshConcept auto&) {}

    /**
     * @brief Function that sets the texture units from the mesh and sends
     * the data to the GPU.
     *
     * The function should take the texture from the mesh (loading them if
     * they are not available in the mesh) and send them to the GPU using the
     * rendering backend.
     *
     * There is no need to check whether the Mesh can provide texture paths,
     * since the function is called only if the mesh has them. However, it is
     * necessary to check whether the mesh has texture images and, in that case,
     * check whether the texture is already loaded.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setTextureUnits(const MeshConcept auto&) {}

    /**
     * @brief Function that sets the mesh uniforms from the mesh.
     *
     * The function should set the uniforms of the mesh (e.g., mesh color,
     * transform) and prepare them to be bound to the shader program.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setMeshUniforms(const MeshConcept auto&) {}

private:
    MeshRenderDerived& derived()
    {
        return static_cast<MeshRenderDerived&>(*this);
    }

    const MeshRenderDerived& derived() const
    {
        return static_cast<const MeshRenderDerived&>(*this);
    }

    void updateAuxiliaryData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if (btu[toUnderlying(VERTICES)] || btu[toUnderlying(WEDGE_TEXCOORDS)] ||
            btu[toUnderlying(TRIANGLES)]) {
            mVertWedgeMap.clear();
            mVertsToDuplicate.clear();
            mFacesToReassign.clear();

            if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                if (mesh.isPerFaceWedgeTexCoordsEnabled()) {
                    countVerticesToDuplicateByWedgeTexCoords(
                        mesh,
                        mVertWedgeMap,
                        mVertsToDuplicate,
                        mFacesToReassign);
                }
            }

            mNumVerts = mesh.vertexNumber() + mVertsToDuplicate.size();
        }

        if constexpr (HasFaces<MeshType>) {
            if (btu[toUnderlying(TRIANGLES)])
                mNumTris = countTriangulatedTriangles(mesh);
            if (btu[toUnderlying(WIREFRAME)])
                nWireframeLines = countPerFaceVertexReferences(mesh);
        }

        if constexpr (HasEdges<MeshType>) {
            if (btu[toUnderlying(EDGES)])
                mNumEdges = mesh.edgeNumber();
        }
    }

    void updateVerticesData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if (btu[toUnderlying(VERTICES)]) {
            // vertex buffer (coordinates)
            derived().setVertexCoordsBuffer(mesh);
        }

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                if (btu[toUnderlying(VERT_NORMALS)]) {
                    // vertex buffer (normals)
                    derived().setVertexNormalsBuffer(mesh);
                }
            }
        }

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                if (btu[toUnderlying(VERT_COLORS)]) {
                    // vertex buffer (colors)
                    derived().setVertexColorsBuffer(mesh);
                }
            }
        }

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                if (btu[toUnderlying(VERT_TEXCOORDS)]) {
                    // vertex buffer (UVs)
                    derived().setVertexTexCoordsBuffer(mesh);
                }
            }
        }
    }

    void updateFacesData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if constexpr (vcl::HasFaces<MeshType>) {
            if (btu[toUnderlying(TRIANGLES)]) {
                // triangle index buffer
                derived().setTriangleIndicesBuffer(mesh);
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    if (btu[toUnderlying(WEDGE_TEXCOORDS)]) {
                        // vertex wedges buffer (duplicated vertices)
                        derived().setWedgeTexCoordsBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                if (vcl::isPerFaceNormalAvailable(mesh)) {
                    if (btu[toUnderlying(TRI_NORMALS)]) {
                        // triangle normal buffer
                        derived().setTriangleNormalsBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceColor<MeshType>) {
                if (vcl::isPerFaceColorAvailable(mesh)) {
                    if (btu[toUnderlying(TRI_COLORS)]) {
                        // triangle color buffer
                        derived().setTriangleColorsBuffer(mesh);
                    }
                }
            }

            // texture indices are stored per face (each face has its own
            // texture index)
            if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                    if (btu[toUnderlying(VERT_TEXCOORDS)]) {
                        // triangle vertex texture indices buffer
                        derived().setVertexTextureIndicesBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                    if (btu[toUnderlying(WEDGE_TEXCOORDS)]) {
                        // triangle wedge texture indices buffer
                        derived().setWedgeTextureIndicesBuffer(mesh);
                    }
                }
            }

            if (btu[toUnderlying(WIREFRAME)]) {
                // wireframe index buffer
                derived().setWireframeIndicesBuffer(mesh);
            }
        }
    }

    void updateEdgesData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if constexpr (vcl::HasEdges<MeshType>) {
            if (btu[toUnderlying(EDGES)]) {
                // edge index buffer
                derived().setEdgeIndicesBuffer(mesh);
            }

            if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
                if (vcl::isPerEdgeNormalAvailable(mesh)) {
                    if (btu[toUnderlying(EDGE_NORMALS)]) {
                        // edge normal buffer
                        derived().setEdgeNormalsBuffer(mesh);
                    }
                }
            }

            if constexpr (vcl::HasPerEdgeColor<MeshType>) {
                if (vcl::isPerEdgeColorAvailable(mesh)) {
                    if (btu[toUnderlying(EDGE_COLORS)]) {
                        // edge color buffer
                        derived().setEdgeColorsBuffer(mesh);
                    }
                }
            }
        }
    }

    void updateMeshData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using enum MRI::Buffers;

        if (btu[toUnderlying(MESH_UNIFORMS)]) {
            // mesh uniforms
            derived().setMeshUniforms(mesh);
        }
    }

    void updateTextureData(
        const MeshConcept auto&       mesh,
        MeshRenderInfo::BuffersBitSet btu)
    {
        using MeshType = MeshRenderDerived::MeshType;
        using enum MRI::Buffers;

        if constexpr (vcl::HasTexturePaths<MeshType>) {
            if (btu[toUnderlying(TEXTURES)]) {
                // textures
                derived().setTextureUnits(mesh);
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_DATA_H
