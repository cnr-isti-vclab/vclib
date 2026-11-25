// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H

#include "detail.h"

#include <vclib/algorithms/mesh/utility.h>

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>
#include <vclib/space/complex.h>

/**
 * @defgroup export_buffer Export Mesh to Buffer Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Export Mesh to Buffer algorithms.
 *
 * They allow to export mesh data to pre-allocated buffers.
 */

namespace vcl {

/**
 * @brief Export the vertex positions of a mesh to a buffer.
 *
 * This function exports the vertex positions of a mesh to a buffer. Vertices
 * are stored in the buffer following the order they appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * the number of positions per vertex).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexPositionsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using IndexType = typename MeshType::IndexType;

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.vertexCount() : numRows;
    for (IndexType i = 0; const auto& p : mesh.vertices() | views::positions) {
        at(buffer, i, 0, NUM_ROWS, 3, storage) = p.x();
        at(buffer, i, 1, NUM_ROWS, 3, storage) = p.y();
        at(buffer, i, 2, NUM_ROWS, 3, storage) = p.z();
        ++i;
    }
}

/**
 * @brief Export the indices of a quad per vertex to a buffer.
 *
 * This function exports the vertex indices of a quad per vertex to a buffer.
 * The buffer must be preallocated with the correct size (number of vertices
 * times 6).
 *
 * The indices are stored in the following order:
 *
 * ```
 * 0 1 2 1 3 2
 * ```
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexQuadIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    using IndexType = typename MeshType::IndexType;

    // creates indices for two triangles (quad) for each vertex
    //
    // 2-------3
    // | \     |
    // |  \    |
    // |   \   |
    // |    \  |
    // |     \ |
    // 0-------1
    //
    // - Triangle 1: [0, 1, 2]
    // - Triangle 2: [1, 3, 2]
    //
    for (IndexType i = 0; const auto& v : mesh.vertices()) {
        const auto baseIdx  = i * 6;
        const auto quadIdx  = i * 4;
        buffer[baseIdx + 0] = quadIdx + 0;
        buffer[baseIdx + 1] = quadIdx + 1;
        buffer[baseIdx + 2] = quadIdx + 2;
        buffer[baseIdx + 3] = quadIdx + 1;
        buffer[baseIdx + 4] = quadIdx + 3;
        buffer[baseIdx + 5] = quadIdx + 2;
        ++i;
    }
}

/**
 * @brief Export into a buffer the sizes of the faces of a Mesh, and return the
 * sum of the sizes.
 *
 * This function could be useful when dealing with polygonal meshes: it exports
 * the sizes of the faces of a mesh to a buffer. Sizes are stored following the
 * order the faces appear in the mesh. The buffer must be preallocated with the
 * correct size (number of faces).
 *
 * The return value is the sum of the sizes of the faces. This value is useful
 * when you need to allocate a buffer to store the vertex indices of the faces
 * (its size is the sum of the face sizes).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @return sum of the sizes of the faces
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
auto faceSizesToBuffer(const MeshType& mesh, auto* buffer)
{
    using IndexType = typename MeshType::IndexType;

    IndexType sum = 0;
    for (IndexType i = 0; const auto& f : mesh.faces()) {
        buffer[i] = f.vertexCount();
        sum += f.vertexCount();
        ++i;
    }
    return sum;
}

/**
 * @brief Export into a buffer the vertex indices for each face of a Mesh. Faces
 * can be polygons.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh to
 * a buffer. Indices are stored consecutively in the buffer, following the order
 * the faces appear in the mesh. The buffer must be preallocated with the
 * correct size (sum of the sizes of the faces).
 *
 * You can use the function @ref vcl::faceSizesToBuffer to get the sizes of the
 * faces and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * std::vector<uint> faceSizes(myMesh.faceCount());
 * uint sum = vcl::faceSizesToBuffer(myMesh, sizes.data());
 * std::vector<uint> faceIndices(sum);
 * vcl::faceVertexIndicesToBuffer(myMesh, faceIndices.data());
 *
 * // read indices for each face
 * uint offset = 0;
 * for (uint i = 0; i < myMesh.faceCount(); ++i) {
 *     uint size = faceSizes[i];
 *     for (uint j = 0; j < size; ++j) {
 *         uint vIdx = faceIndices[offset + j];
 *         // do something with the vertex index
 *     }
 *     offset += size;
 * }
 * @endcode
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceVertexIndicesToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    bool            getIndicesAsIfContainerCompact = true)
{
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    detail::checkBufferIndexCapacity<BufferValueType>(
        getIndicesAsIfContainerCompact ? mesh.vertexCount() :
                                         mesh.vertexContainerSize());

    using IndexType = typename MeshType::IndexType;

    auto vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    for (IndexType i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < f.vertexCount(); ++j) {
            buffer[i] = static_cast<BufferValueType>(vIndex(f, j));
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each face of a Mesh. Faces
 * can be polygons, and the number of output columns can be set by the user with
 * the `largestFaceSize` parameter.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh to
 * a buffer. Indices are stored following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of faces
 * times `largestFaceSize`). For each face that has less vertices than the
 * largest face size, the remaining indices are set to the sentinel value of the
 * buffer element type (i.e. `static_cast<BufferValueType>(-1)`, which is `-1`
 * for signed integer types, `-1.0` for floating-point types, or the maximum
 * representable value for unsigned integer types).
 *
 * For triangle meshes, you can set `largestFaceSize` to 3. For polygonal
 * meshes, you can use the function @ref vcl::largestFaceSize to get the largest
 * face size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceIndices(myMesh.faceCount(), lfs);
 * vcl::faceVertexIndicesToBuffer(
 *     myMesh, faceIndices.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFaceSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceVertexIndicesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestFaceSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    bool                         getIndicesAsIfContainerCompact = true,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(
        getIndicesAsIfContainerCompact ? mesh.vertexCount() :
                                         mesh.vertexContainerSize());

    using IndexType = typename MeshType::IndexType;

    auto vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.faceCount() : numRows;

    const auto SENTINEL = bufferSentinel<BufferValueType>();

    for (IndexType i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < largestFaceSize; ++j) {
            at(buffer, i, j, NUM_ROWS, largestFaceSize, storage) =
                j < f.vertexCount() ?
                    static_cast<BufferValueType>(vIndex(f, j)) :
                    SENTINEL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the vertex indices for each triangle computed
 * by triangulating the faces of a Mesh.
 *
 * This function exports the vertex indices of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Indices are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 3).
 *
 * You can use the function @ref vcl::triangulatedFaceCount to get the number of
 * resulting triangles and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint numTris = vcl::triangulatedFaceCount(myMesh);
 * Eigen::MatrixXi triIndices(numTris, 3);
 * vcl::TriPolyIndexBiMap indexMap;
 * vcl::triangulatedFaceVertexIndicesToBuffer(
 *     myMesh, triIndices.data(), indexMap, MatrixStorageType::COLUMN_MAJOR,
 *     numTris);
 * @endcode
 *
 * The input indexMap is used to map each triangle to the face index. If the
 * storage of the buffer is column major, the number of resulting triangles
 * (that should be known when calling this function) should be given as input.
 * If the number of resulting triangles is not given, the function will compute
 * it again.
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[out] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numTriangles: number of resulting triangles (necessary only if
 * the storage is column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceVertexIndicesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    TriPolyIndexBiMap&           indexMap     = detail::indexMap,
    MatrixStorageType            storage      = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numTriangles = MeshType::NULL_INDEX,
    bool                         getIndicesAsIfContainerCompact = true)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(
        getIndicesAsIfContainerCompact ? mesh.vertexCount() :
                                         mesh.vertexContainerSize());

    using IndexType = typename MeshType::IndexType;

    auto vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    // there will be at least a triangle for each polygon
    indexMap.clear();
    indexMap.reserve(mesh.faceCount(), mesh.faceContainerSize());

    if constexpr (TriangleMeshConcept<MeshType>) {
        // construct the indexMap, which maps each triangle to the face index
        for (IndexType t = 0; const auto& f : mesh.faces()) {
            // map the ith triangle to the f face
            indexMap.insert(t, f.index());
            ++t;
        }

        faceVertexIndicesToBuffer(
            mesh, buffer, 3, storage, getIndicesAsIfContainerCompact);
    }
    else {
        // if the user did not give the number of triangles, and the buffer
        // storage is column major, we need to compute the number of resulting
        // triangles
        if (numTriangles == MeshType::NULL_INDEX &&
            storage == MatrixStorageType::COLUMN_MAJOR &&
            mesh.faceCount() > 0) {
            numTriangles = triangulatedFaceCount(mesh);
        }
        for (IndexType t = 0; const auto& f : mesh.faces()) {
            std::vector<uint> vind = vcl::earCut(f);

            // for each triangle of the triangulation (t is the triangle index)
            for (uint vi = 0; vi < vind.size(); vi += 3) {
                // map the t-th triangle to the f polygonal face
                indexMap.insert(t, f.index());

                for (uint k = 0; k < 3; ++k)
                    at(buffer, t, k, numTriangles, 3, storage) =
                        static_cast<BufferValueType>(vIndex(f, vind[vi + k]));

                ++t;
            }
        }
    }

    if (&indexMap == &detail::indexMap) {
        // if the user did not give an indexMap, we clear it at the end of the
        // function to keep the global one empty for the next call.
        indexMap.clear();
    }
}

/**
 * @brief Export into a buffer the vertex indices for each edge of a Mesh.
 *
 * This function exports the vertex indices of the edges of a mesh to a
 * buffer. Indices are stored following the order the edges appear in the mesh.
 * The buffer must be preallocated with the correct size (number of edges times
 * 2).
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeVertexIndicesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    bool                         getIndicesAsIfContainerCompact = true,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(
        getIndicesAsIfContainerCompact ? mesh.vertexCount() :
                                         mesh.vertexContainerSize());

    using IndexType = typename MeshType::IndexType;

    auto vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a edge (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.edgeCount() : numRows;

    for (IndexType i = 0; const auto& e : mesh.edges()) {
        at(buffer, i, 0, NUM_ROWS, 2, storage) =
            static_cast<BufferValueType>(vIndex(e, 0));
        at(buffer, i, 1, NUM_ROWS, 2, storage) =
            static_cast<BufferValueType>(vIndex(e, 1));

        ++i;
    }
}

/**
 * @brief Export into a buffer the vertex indices for each edge that composes
 * the wireframe of the Mesh (i.e., the edges of the faces).
 *
 * This function exports the vertex indices of the wireframe edges of a mesh to
 * a buffer. Indices are stored following the order the edges appear in the
 * faces. The buffer must be preallocated with the correct size (number of
 * references to vertices in the mesh faces times 2 - see @ref
 * faceVertexReferencesCount).
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of references to vertices in the mesh faces times 2) - used only when
 * storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void wireframeVertexIndicesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    bool                         getIndicesAsIfContainerCompact = true,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(
        getIndicesAsIfContainerCompact ? mesh.vertexCount() :
                                         mesh.vertexContainerSize());

    using IndexType = typename MeshType::IndexType;

    auto vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a edge (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    const auto NUM_ROWS = numRows == MeshType::NULL_INDEX ?
                              faceVertexReferencesCount(mesh) :
                              numRows;

    for (IndexType i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < f.vertexCount(); ++j) {
            IndexType v0 = vIndex(f, j);
            IndexType v1 = vIndex(f, (j + 1) % f.vertexCount());

            at(buffer, i, 0, NUM_ROWS, 2, storage) =
                static_cast<BufferValueType>(v0);
            at(buffer, i, 1, NUM_ROWS, 2, storage) =
                static_cast<BufferValueType>(v1);

            ++i;
        }
    }
}

/**
 * @brief Export the selection status of the elements identified by
 * `ELEM_ID` of a mesh to a buffer.
 *
 * This function exports the selection status of the elements identified by
 * `ELEM_ID` of a mesh to a buffer. Values are stored in the buffer
 * following the order the elements appear in the mesh. The buffer must be
 * preallocated with the correct size (number of elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.elementCount<ElemId::VERTEX>());
 * vcl::elementSelectionToBuffer<ElemId::VERTEX>(myMesh, vec.data());
 * @endcode
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this
 * function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void elementSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    using IndexType = typename MeshType::IndexType;

    for (IndexType i = 0; const auto& e : mesh.template elements<ELEM_ID>()) {
        buffer[i] = e.selected();
        ++i;
    }
}

/**
 * @brief Export the selection status of the vertices of a mesh to a buffer.
 *
 * This function exports the selection status of the vertices of a mesh to a
 * buffer. Values are stored in the buffer following the order the vertices
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.vertexCount());
 * vcl::vertexSelectionToBuffer(myMesh, vec.data());
 * @endcode
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Export the selection status of the faces of a mesh to a buffer.
 *
 * This function exports the selection status of the faces of a mesh to a
 * buffer. Values are stored in the buffer following the order the faces appear
 * in the mesh. The buffer must be preallocated with the correct size (number of
 * elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.faceCount());
 * vcl::faceSelectionToBuffer(myMesh, vec.data());
 * @endcode
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the selection status of the edges of a mesh to a buffer.
 *
 * This function exports the selection status of the edges of a mesh to a
 * buffer. Values are stored in the buffer following the order the edges appear
 * in the mesh. The buffer must be preallocated with the correct size (number of
 * elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.edgeCount());
 * vcl::edgeSelectionToBuffer(myMesh, vec.data());
 * @endcode
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::EDGE>(mesh, buffer);
}

/**
 * @brief Export the element normals identified by `ELEM_ID` of a mesh to a
 * buffer.
 *
 * This function exports the element normals identified by `ELEM_ID` of a mesh
 * to a buffer. Normals are stored in the buffer following the order the
 * elements appear in the mesh. The buffer must be preallocated with the correct
 * size (number of elements times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] normalize: if true, the normals will be normalized before being
 * stored in the buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void elementNormalsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    bool                         normalize = false,
    MatrixStorageType            storage   = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows   = MeshType::NULL_INDEX)
{
    using namespace detail;
    using IndexType = typename MeshType::IndexType;

    requireElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    const auto NUM_ROWS = numRows == MeshType::NULL_INDEX ?
                              mesh.template count<ELEM_ID>() :
                              numRows;

    for (IndexType i = 0;
         auto      n : mesh.template elements<ELEM_ID>() | views::normals) {
        if (normalize)
            n.normalize();
        at(buffer, i, 0, NUM_ROWS, 3, storage) = n.x();
        at(buffer, i, 1, NUM_ROWS, 3, storage) = n.y();
        at(buffer, i, 2, NUM_ROWS, 3, storage) = n.z();

        ++i;
    }
}

/**
 * @brief Export the vertex normals of a mesh to a buffer.
 *
 * This function exports the vertex normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] normalize: if true, the normals will be normalized before being
 * stored in the buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexNormalsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    bool                         normalize = false,
    MatrixStorageType            storage   = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows   = MeshType::NULL_INDEX)
{
    elementNormalsToBuffer<ElemId::VERTEX>(
        mesh, buffer, normalize, storage, numRows);
}

/**
 * @brief Export the face normals of a mesh to a buffer.
 *
 * This function exports the face normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] normalize: if true, the normals will be normalized before being
 * stored in the buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceNormalsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    bool                         normalize = false,
    MatrixStorageType            storage   = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows   = MeshType::NULL_INDEX)
{
    elementNormalsToBuffer<ElemId::FACE>(
        mesh, buffer, normalize, storage, numRows);
}

/**
 * @brief Export into a buffer the normals for each triangle computed by
 * triangulating the faces of a Mesh.
 *
 * This function exports the normals of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Normals are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 3).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceVertexIndicesToBuffer function to get the index map. You
 * can use the function @ref vcl::triangulatedFaceCount to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] normalize: if true, the normals will be normalized before being
 * stored in the buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of triangles in the map) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceNormalsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    const TriPolyIndexBiMap&     indexMap,
    bool                         normalize = false,
    MatrixStorageType            storage   = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows   = MeshType::NULL_INDEX)
{
    using namespace detail;

    requireFaceNormal(mesh);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? indexMap.triangleCount() : numRows;

    for (const auto& f : mesh.faces()) {
        auto n = f.normal();
        if (normalize)
            n.normalize();
        auto first = indexMap.triangleBegin(f.index());
        auto last  = first + indexMap.triangleCount(f.index());
        for (auto t = first; t < last; ++t) {
            at(buffer, t, 0, NUM_ROWS, 3, storage) = n.x();
            at(buffer, t, 1, NUM_ROWS, 3, storage) = n.y();
            at(buffer, t, 2, NUM_ROWS, 3, storage) = n.z();
        }
    }
}

/**
 * @brief Export the edge normals of a mesh to a buffer.
 *
 * This function exports the edge normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] normalize: if true, the normals will be normalized before being
 * stored in the buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeNormalsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    bool                         normalize = false,
    MatrixStorageType            storage   = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows   = MeshType::NULL_INDEX)
{
    elementNormalsToBuffer<ElemId::EDGE>(
        mesh, buffer, normalize, storage, numRows);
}

/**
 * @brief Export the element colors identified by `ELEM_ID` of a mesh to a
 * buffer having a value for each color component (RGBA).
 *
 * This function exports the element colors identified by `ELEM_ID` of a mesh
 * to a buffer. Colors are stored in the buffer following the order the elements
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using IndexType = typename MeshType::IndexType;

    requireElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    const auto NUM_ROWS = numRows == MeshType::NULL_INDEX ?
                              mesh.template count<ELEM_ID>() :
                              numRows;

    for (IndexType   i = 0;
         const auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        at(buffer, i, 0, NUM_ROWS, 4, storage) = R_INT ? c.red() : c.redF();
        at(buffer, i, 1, NUM_ROWS, 4, storage) = R_INT ? c.green() : c.greenF();
        at(buffer, i, 2, NUM_ROWS, 4, storage) = R_INT ? c.blue() : c.blueF();
        at(buffer, i, 3, NUM_ROWS, 4, storage) = R_INT ? c.alpha() : c.alphaF();

        ++i;
    }
}

/**
 * @brief Export the element colors identified by `ELEM_ID` of a mesh to a
 * buffer having a value for each color (the color is packed in a single 32 bit
 * value using the provided format).
 *
 * This function exports the element colors identified by `ELEM_ID` of a mesh to
 * a buffer. Colors are stored in the buffer following the order the elements
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible when
 * the mesh has deleted elements. To be sure to have a direct correspondence,
 * compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    using IndexType = typename MeshType::IndexType;

    requireElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    for (IndexType   i = 0;
         const auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        switch (colorFormat) {
            using enum Color::Format;
        case ABGR: buffer[i] = c.abgr(); break;
        case ARGB: buffer[i] = c.argb(); break;
        case RGBA: buffer[i] = c.rgba(); break;
        case BGRA: buffer[i] = c.bgra(); break;
        }
        ++i;
    }
}

/**
 * @brief Export the vertex colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the vertex colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementColorsToBuffer<ElemId::VERTEX>(
        mesh, buffer, storage, representation, numRows);
}

/**
 * @brief Export the vertex colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the vertex colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible when
 * the mesh has deleted vertices. To be sure to have a direct correspondence,
 * compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::VERTEX>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the face colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the face colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementColorsToBuffer<ElemId::FACE>(
        mesh, buffer, storage, representation, numRows);
}

/**
 * @brief Export into a buffer the color values (RGBA) for each triangle
 * computed by triangulating the faces of a Mesh.
 *
 * This function exports the colors of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Colors are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 4).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceVertexIndicesToBuffer function to get the index map. You
 * can use the function @ref vcl::triangulatedFaceCount to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of triangles in the map) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceColorsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    MatrixStorageType        storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation    representation = Color::Representation::INT_0_255,
    typename MeshType::IndexType numRows    = MeshType::NULL_INDEX)
{
    using namespace detail;

    requireFaceColor(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? indexMap.triangleCount() : numRows;

    for (const auto& f : mesh.faces()) {
        const auto& c     = f.color();
        auto        first = indexMap.triangleBegin(f.index());
        auto        last  = first + indexMap.triangleCount(f.index());
        for (auto t = first; t < last; ++t) {
            at(buffer, t, 0, NUM_ROWS, 4, storage) = R_INT ? c.red() : c.redF();
            at(buffer, t, 1, NUM_ROWS, 4, storage) =
                R_INT ? c.green() : c.greenF();
            at(buffer, t, 2, NUM_ROWS, 4, storage) =
                R_INT ? c.blue() : c.blueF();
            at(buffer, t, 3, NUM_ROWS, 4, storage) =
                R_INT ? c.alpha() : c.alphaF();
        }
    }
}

/**
 * @brief Export the face colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the face colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::FACE>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the colors for each triangle computed by triangulating the
 * faces of a Mesh to a buffer having a value for each color (the color is
 * packed in a single 32 bit value using the provided format)..
 *
 * This function exports the colors of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Colors are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles*).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceVertexIndicesToBuffer function to get the index map. You
 * can use the function @ref vcl::triangulatedFaceCount to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceColorsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    Color::Format            colorFormat)
{
    requireElementComponent<ElemId::FACE, CompId::COLOR>(mesh);

    for (const auto& f : mesh.faces()) {
        const auto& c     = f.color();
        auto        first = indexMap.triangleBegin(f.index());
        auto        last  = first + indexMap.triangleCount(f.index());
        for (auto t = first; t < last; ++t) {
            switch (colorFormat) {
                using enum Color::Format;
            case ABGR: buffer[t] = c.abgr(); break;
            case ARGB: buffer[t] = c.argb(); break;
            case RGBA: buffer[t] = c.rgba(); break;
            case BGRA: buffer[t] = c.bgra(); break;
            }
        }
    }
}

/**
 * @brief Export the edge colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the edge colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementColorsToBuffer<ElemId::EDGE>(
        mesh, buffer, storage, representation, numRows);
}

/**
 * @brief Export the edge colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the edge colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::EDGE>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the element quality identified by `ELEM_ID` of a mesh to a
 * buffer.
 *
 * This function exports the element quality identified by `ELEM_ID` of a mesh
 * to a buffer. Quality values are stored in the buffer following the order the
 * elements appear in the mesh. The buffer must be preallocated with the correct
 * size (number of elements).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible when
 * the mesh has deleted elements. To be sure to have a direct correspondence,
 * compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void elementQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    using IndexType = typename MeshType::IndexType;

    requireElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    for (IndexType   i = 0;
         const auto& q : mesh.template elements<ELEM_ID>() | views::quality) {
        buffer[i] = q;
        ++i;
    }
}

/**
 * @brief Export the vertex quality of a mesh to a buffer.
 *
 * This function exports the vertex quality of a mesh to a buffer. Quality
 * values are stored in the buffer following the order the vertices appear in
 * the mesh. The buffer must be preallocated with the correct size (number of
 * vertices).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible when
 * the mesh has deleted vertices. To be sure to have a direct correspondence,
 * compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Export the face quality of a mesh to a buffer.
 *
 * This function exports the face quality of a mesh to a buffer. Quality values
 * are stored in the buffer following the order the faces appear in the mesh.
 * The buffer must be preallocated with the correct size (number of faces).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void faceQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the edge quality of a mesh to a buffer.
 *
 * This function exports the edge quality of a mesh to a buffer. Quality values
 * are stored in the buffer following the order the edges appear in the mesh.
 * The buffer must be preallocated with the correct size (number of edges).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::EDGE>(mesh, buffer);
}

/**
 * @brief Export the vertex tangent of a mesh to a buffer.
 *
 * This function exports the vertex tangent of a mesh to a buffer. Tangents are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size.
 *
 * The number of components for each tangent stored in the buffer depends on
 * the `storeHandednessAsW` parameter: if true, 4 components are stored (xyz
 * and w for the handedness: -1 if the bitangent is computed as cross product of
 * normal and tangent, +1 otherwise); otherwise only the xyz components are
 * stored.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexTangentsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    bool                         storeHandednessAsW = true,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using IndexType = typename MeshType::IndexType;

    requireVertexTangent(mesh);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.vertexCount() : numRows;
    const uint NUM_COLS = storeHandednessAsW ? 4 : 3;

    for (IndexType i = 0; const auto& v : mesh.vertices()) {
        at(buffer, i, 0, NUM_ROWS, NUM_COLS, storage) = v.tangent().x();
        at(buffer, i, 1, NUM_ROWS, NUM_COLS, storage) = v.tangent().y();
        at(buffer, i, 2, NUM_ROWS, NUM_COLS, storage) = v.tangent().z();
        if (storeHandednessAsW) {
            at(buffer, i, 3, NUM_ROWS, NUM_COLS, storage) =
                v.tangentRightHanded() ? 1.0 : -1.0;
        }

        ++i;
    }
}

/**
 * @brief Export the vertex texcoords of a mesh to a buffer.
 *
 * This function exports the vertex texcoords of a mesh to a buffer. Texcoords
 * are stored in the buffer following the order the vertices appear in the mesh.
 * The buffer must be preallocated with the correct size (number of vertices
 * times 2).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexTexCoordsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using IndexType = typename MeshType::IndexType;

    requireVertexTexCoord(mesh);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.vertexCount() : numRows;

    for (IndexType i = 0; const auto& t : mesh.vertices() | views::texCoords) {
        at(buffer, i, 0, NUM_ROWS, 2, storage) = t.u();
        at(buffer, i, 1, NUM_ROWS, 2, storage) = t.v();

        ++i;
    }
}

/**
 * @brief Export the vertex material indices of a mesh to a buffer.
 *
 * This function exports the vertex material indices of a mesh to a buffer.
 * Indices are stored in the buffer following the order the vertices appear in
 * the mesh. The buffer must be preallocated with the correct size (number of
 * vertices).
 *
 * For vertices that have no material assigned (`vcl::USHORT_NULL`), the
 * corresponding buffer entry is set to the sentinel value of the buffer element
 * type (i.e. `static_cast<BufferValueType>(-1)`, which is `-1` for signed
 * integer types, `-1.0` for floating-point types, or the maximum representable
 * value for unsigned integer types).
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexMaterialIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    using IndexType       = typename MeshType::IndexType;
    using BufferValueType = std::remove_cvref_t<decltype(buffer[0])>;
    const auto SENTINEL   = detail::bufferSentinel<BufferValueType>();

    requireVertexMaterialIndex(mesh);

    for (IndexType i = 0; const auto& v : mesh.vertices()) {
        auto mi = v.materialIndex();
        buffer[i] =
            (mi == USHORT_NULL) ? SENTINEL : static_cast<BufferValueType>(mi);
        ++i;
    }
}

/**
 * @brief Export the vertex material indices of a mesh into a buffer that has
 * a material index for each face of the mesh (as if the indices were face
 * material indices).
 *
 * This function exports the vertex material indices of a mesh to a buffer. The
 * indices are stored in the buffer following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of
 * faces).
 *
 * For each face, the function takes the material index of the first vertex of
 * the face and stores it in the buffer.
 *
 * If the first vertex of a face has no material assigned (`vcl::USHORT_NULL`),
 * the corresponding entry is set to the sentinel value of the buffer element
 * type (i.e. `static_cast<BufferValueType>(-1)`, which is `-1` for signed
 * integer types, `-1.0` for floating-point types, or the maximum representable
 * value for unsigned integer types).
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void vertexMaterialIndicesAsFaceMaterialIndicesToBuffer(
    const MeshType& mesh,
    auto*           buffer)
{
    using IndexType       = typename MeshType::IndexType;
    using BufferValueType = std::remove_cvref_t<decltype(buffer[0])>;
    const auto SENTINEL   = detail::bufferSentinel<BufferValueType>();

    requireVertexMaterialIndex(mesh);

    for (IndexType i = 0; const auto& f : mesh.faces()) {
        auto mi = f.vertex(0).materialIndex();
        buffer[i] =
            (mi == USHORT_NULL) ? SENTINEL : static_cast<BufferValueType>(mi);
        ++i;
    }
}

/**
 * @brief Export the vertex material indices of a mesh into a buffer that has
 * a material index for each triangle of the mesh (as if the indices were face
 * material indices).
 *
 * This function exports the vertex material indices of a mesh to a buffer. The
 * indices are stored in the buffer following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of
 * triangles).
 *
 * For each triangle computed from the triangulation of a face, the function
 * takes the material index of the first vertex of the face that contains the
 * triangle and stores it in the buffer.
 *
 * If the first vertex of a face has no material assigned (`vcl::USHORT_NULL`),
 * the corresponding entries are set to the sentinel value of the buffer element
 * type (i.e. `static_cast<BufferValueType>(-1)`, which is `-1` for signed
 * integer types, `-1.0` for floating-point types, or the maximum representable
 * value for unsigned integer types).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceVertexIndicesToBuffer function to get the index map.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void vertexMaterialIndicesAsTriangulatedFaceMaterialIndicesToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap)
{
    using BufferValueType = std::remove_cvref_t<decltype(buffer[0])>;
    const auto SENTINEL   = detail::bufferSentinel<BufferValueType>();

    requireVertexMaterialIndex(mesh);

    for (const auto& f : mesh.faces()) {
        auto mi = f.vertex(0).materialIndex();
        auto val =
            (mi == USHORT_NULL) ? SENTINEL : static_cast<BufferValueType>(mi);
        auto first = indexMap.triangleBegin(f.index());
        auto last  = first + indexMap.triangleCount(f.index());
        for (auto t = first; t < last; ++t) {
            buffer[t] = val;
        }
    }
}

/**
 * @brief Export into a buffer the per face wedge texture coordinates of a mesh.
 * Faces can be polygons, and the number of output columns in the buffer can be
 * controlled by the user with the `largestFaceSize` parameter.
 *
 * This function exports the per face wedge texture coordinates of a mesh to a
 * buffer. Texture coordinates are stored in the buffer following the order the
 * faces appear in the mesh. The buffer must be preallocated with the correct
 * size (number of faces times `largestFaceSize` times 2). For each face that
 * has less wedge texcoords than `largestFaceSize`, the remaining columns are
 * filled with 0.
 *
 * For triangle meshes, you can set `largestFaceSize` to 3. For polygonal
 * meshes, you can use the function @ref vcl::largestFaceSize to get the largest
 * face size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceIndices(myMesh.faceCount(), lfs * 2);
 * vcl::faceWedgeTexCoordsToBuffer(
 *     myMesh, faceIndices.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFaceSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceWedgeTexCoordsToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestFaceSize = 3,
    MatrixStorageType            storage         = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows         = MeshType::NULL_INDEX)
{
    using IndexType = typename MeshType::IndexType;

    requireFaceWedgeTexCoords(mesh);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.faceCount() : numRows;

    for (IndexType i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < largestFaceSize * 2; ++j) {
            uint fi    = j / 2;
            auto index = i * largestFaceSize * 2 + j;
            if (storage == MatrixStorageType::COLUMN_MAJOR)
                index = j * NUM_ROWS + i;
            if (fi < f.vertexCount()) {
                const auto& w = f.wedge(fi).texCoord();
                if (j % 2 == 0)
                    buffer[index] = w.u();
                else
                    buffer[index] = w.v();
            }
            else {
                buffer[index] = 0;
            }
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the per face material indices of a mesh.
 *
 * This function exports the per face material indices of a mesh to a buffer.
 * Material indices are stored in the buffer following the order the faces
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of faces).
 *
 * For faces that have no material assigned (`vcl::USHORT_NULL`), the
 * corresponding buffer entry is set to the sentinel value of the buffer element
 * type (i.e. `static_cast<BufferValueType>(-1)`, which is `-1` for signed
 * integer types, `-1.0` for floating-point types, or the maximum representable
 * value for unsigned integer types).
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceMaterialIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    using IndexType       = typename MeshType::IndexType;
    using BufferValueType = std::remove_cvref_t<decltype(buffer[0])>;
    const auto SENTINEL   = detail::bufferSentinel<BufferValueType>();

    requireFaceMaterialIndex(mesh);

    for (IndexType i = 0; const auto& f : mesh.faces()) {
        auto mi = f.materialIndex();
        buffer[i] =
            (mi == USHORT_NULL) ? SENTINEL : static_cast<BufferValueType>(mi);
        ++i;
    }
}

/**
 * @brief Export into a buffer the per triangle material indices of a mesh.
 * Triangles are computed by triangulating the faces of the mesh.
 *
 * This function exports the per triangle material indices of a mesh to a
 * buffer. Material indices are stored in the buffer following the order the
 * faces appear in the mesh. The buffer must be preallocated with the correct
 * size (number of triangles).
 *
 * For triangles derived from faces that have no material assigned
 * (`vcl::USHORT_NULL`), the corresponding buffer entries are set to the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceVertexIndicesToBuffer function to get the index map. You
 * can use the function @ref vcl::triangulatedFaceCount to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceMaterialIndicesToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap)
{
    using BufferValueType = std::remove_cvref_t<decltype(buffer[0])>;
    const auto SENTINEL   = detail::bufferSentinel<BufferValueType>();

    requireFaceMaterialIndex(mesh);

    for (const auto& f : mesh.faces()) {
        auto mi = f.materialIndex();
        auto val =
            (mi == USHORT_NULL) ? SENTINEL : static_cast<BufferValueType>(mi);
        auto first = indexMap.triangleBegin(f.index());
        auto last  = first + indexMap.triangleCount(f.index());
        for (auto t = first; t < last; ++t) {
            buffer[t] = val;
        }
    }
}

/**
 * @brief Export wedge texture coordinates to a buffer of the duplicated
 * vertex texture coordinates.
 *
 * Given the list of vertices to duplicate, this function exports to the given
 * buffer the wedge texture coordinates as if they were vertex texture
 * coordinates, and appending only the texture coordinates of the vertices to
 * duplicate.
 *
 * Typical usage of this function is after the @ref
 * verticesToDuplicateByWedgeTexCoordsCount function:
 *
 * @code{.cpp}
 *
 * using IndexType = IndexType<MeshType>;
 * std::vector<std::pair<IndexType, uint>> vertWedgeMap;
 * std::list<IndexType> vertsToDuplicate;
 * std::list<std::list<std::pair<IndexType, uint>>> facesToReassign;
 *
 * IndexType nV = verticesToDuplicateByWedgeTexCoordsCount(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<double> buffer((mesh.vertexCount() + nV) * 2);
 * wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(mesh, vertWedgeMap,
 *     facesToReassign, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices times 2).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the wedge texture coordinates.
 * @param[in] vertWedgeMap: The map from non-duplicated vertex index to face
 * index and wedge index in the face.
 * @param[in] facesToReassign: The list of lists of pairs face/vertex index in
 * the face that must be reassigned to the duplicated vertices. Each list of
 * pairs is the list of faces that must be reassigned to the corresponding
 * duplicated vertex.
 * @param[out] buffer: The buffer where to export the vertex wedge texture
 * coordinates.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<FaceMeshConcept MeshType>
void wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
    const MeshType&                                          mesh,
    const std::vector<std::pair<IndexType<MeshType>, uint>>& vertWedgeMap,
    const std::list<std::list<std::pair<IndexType<MeshType>, uint>>>&
                      facesToReassign,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR)
{
    using namespace detail;
    using TexCoordType = TexCoordType<WedgeType<FaceType<MeshType>>>;
    using IndexType    = MeshType::IndexType;

    requireFaceWedgeTexCoords(mesh);

    const auto NUM_ROWS = mesh.vertexCount() + facesToReassign.size();

    // first export the texcoords of the non-duplicated vertices, using the
    // vertWedgeMap to get the texcoord index in the face
    IndexType vi = 0; // current vertex (or current row in the matrix)
    for (const auto& v : mesh.vertices()) {
        TexCoordType w;
        auto         fInd = vertWedgeMap[v.index()].first;
        auto         wInd = vertWedgeMap[v.index()].second;

        // check if the vertex is referenced
        if (fInd != MeshType::NULL_INDEX && wInd != UINT_NULL) {
            w = mesh.face(fInd).wedge(wInd).texCoord();
        }
        at(buffer, vi, 0, NUM_ROWS, 2, storage) = w.u();
        at(buffer, vi, 1, NUM_ROWS, 2, storage) = w.v();

        ++vi;
    }

    // then append the texcoords of the duplicated vertices, that can be found
    // by looking into the any of the facesToReassign element lists
    for (const auto& list : facesToReassign) {
        assert(list.begin() != list.end());
        const auto& p    = list.front();
        auto        fInd = p.first;
        auto        wInd = p.second;

        const auto& w = mesh.face(fInd).wedge(wInd).texCoord();
        at(buffer, vi, 0, NUM_ROWS, 2, storage) = w.u();
        at(buffer, vi, 1, NUM_ROWS, 2, storage) = w.v();

        ++vi;
    }
}

/**
 * @brief Export face material indices to a buffer of the duplicated vertex
 * material indices.
 *
 * Given the list of vertices to duplicate, this function exports to the given
 * buffer the face material indices as if they were vertex material
 * indices, and appending only the material indices of the vertices to
 * duplicate.
 *
 * Typical usage of this function is after the @ref
 * verticesToDuplicateByWedgeTexCoordsCount function:
 *
 * @code{.cpp}
 *
 * using IndexType = IndexType<MeshType>;
 * std::vector<std::pair<IndexType, uint>> vertWedgeMap;
 * std::list<IndexType> vertsToDuplicate;
 * std::list<std::list<std::pair<IndexType, uint>>> facesToReassign;
 *
 * IndexType nV = verticesToDuplicateByWedgeTexCoordsCount(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<ushort> buffer(mesh.vertexCount() + nV);
 * faceMaterialIndicesAsDuplicatedVertexMaterialIndicesToBuffer(mesh,
 *     vertWedgeMap, facesToReassign, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices).
 *
 * For vertices originating from faces that have no material assigned
 * (`vcl::USHORT_NULL`), the corresponding buffer entries are set to the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the face material indices.
 * @param[in] vertWedgeMap: The map from non-duplicated vertex index to face
 * index and wedge index in the face.
 * @param[in] facesToReassign: The list of lists of pairs face/vertex index in
 * the face that must be reassigned to the duplicated vertices. Each list of
 * pairs is the list of faces that must be reassigned to the corresponding
 * duplicated vertex.
 * @param[out] buffer: The buffer where to export the vertex material indices.
 *
 * @ingroup append_replace_to_buffer
 */
template<FaceMeshConcept MeshType>
void faceMaterialIndicesAsDuplicatedVertexMaterialIndicesToBuffer(
    const MeshType&                                          mesh,
    const std::vector<std::pair<IndexType<MeshType>, uint>>& vertWedgeMap,
    const std::list<std::list<std::pair<IndexType<MeshType>, uint>>>&
          facesToReassign,
    auto* buffer)
{
    using IndexType       = typename MeshType::IndexType;
    using BufferValueType = std::remove_cvref_t<decltype(buffer[0])>;
    const auto SENTINEL   = detail::bufferSentinel<BufferValueType>();

    requireFaceMaterialIndex(mesh);

    const auto VERT_NUM = mesh.vertexCount() + facesToReassign.size();

    // first export the material indices of the non-duplicated vertices, using
    // the vertWedgeMap to get the material index in the face
    IndexType vi = 0; // current vertex (or current row in the matrix)
    for (const auto& v : mesh.vertices()) {
        auto fInd = vertWedgeMap[v.index()].first;
        if (fInd != MeshType::NULL_INDEX) {
            auto ti    = mesh.face(fInd).materialIndex();
            buffer[vi] = (ti == USHORT_NULL) ? SENTINEL :
                                               static_cast<BufferValueType>(ti);
        }
        else {
            buffer[vi] = SENTINEL;
        }
        ++vi;
    }

    // then append the material indices of the duplicated vertices, that can be
    // found by looking into the any of the facesToReassign element lists
    for (const auto& list : facesToReassign) {
        assert(list.begin() != list.end());
        const auto& p    = list.front();
        auto        fInd = p.first;

        if (fInd != MeshType::NULL_INDEX) {
            auto ti    = mesh.face(fInd).materialIndex();
            buffer[vi] = (ti == USHORT_NULL) ? SENTINEL :
                                               static_cast<BufferValueType>(ti);
        }
        else {
            buffer[vi] = SENTINEL;
        }
        ++vi;
    }
}

/**
 * @brief Export into a buffer the adjacent vertex indices for each vertex of a
 * Mesh. The number of adjacent vertices for each vertex can be different, so
 * the user must provide the size of the largest adjacency list with the
 * `largestAdjacentVerticesSize` parameter.
 * For vertices that have less adjacent vertices than
 * `largestAdjacentVerticesSize`, or for entries that have no adjacent vertex
 * (e.g. boundary vertices), the entries are filled with the sentinel value of
 * the buffer element type (i.e. `static_cast<BufferValueType>(-1)`, which is
 * `-1` for signed integer types, `-1.0` for floating-point types, or the
 * maximum representable value for unsigned integer types).
 *
 * You can use the function @ref vcl::largestVertexAdjacentVerticesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestVertexAdjacentVerticesCount(myMesh);
 * Eigen::MatrixXi vertexAdj(myMesh.vertexCount(), lva);
 * vcl::vertexAdjacentVerticesToBuffer(
 *    myMesh, vertexAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentVerticesSize: size of the largest per-vertex vertex
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexAdjacentVerticesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentVerticesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(mesh.vertexCount());

    using IndexType = typename MeshType::IndexType;

    requireVertexCompactness(mesh);
    requireVertexAdjacentVertices(mesh);

    const auto NUM_ROWS =
        numRows == MeshType::NULL_INDEX ? mesh.vertexCount() : numRows;

    const uint NUM_COLS = largestAdjacentVerticesSize;
    const auto SENTINEL = bufferSentinel<BufferValueType>();

    for (IndexType i = 0; const auto& v : mesh.vertices()) {
        uint adjIndex = 0;
        for (auto idx : v.adjVertexIndices()) {
            at(buffer, i, adjIndex, NUM_ROWS, NUM_COLS, storage) =
                idx == MeshType::NULL_INDEX ? SENTINEL :
                                              static_cast<BufferValueType>(idx);
            ++adjIndex;
        }
        // fill the remaining entries with the sentinel value
        for (; adjIndex < NUM_COLS; ++adjIndex) {
            at(buffer, i, adjIndex, NUM_ROWS, NUM_COLS, storage) = SENTINEL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the adjacent faces indices for each ELEM_ID
 * element of a Mesh. The number of adjacent faces for each ELEM_ID can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentFacesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentFacesSize`, or for entries that have no
 * adjacent face (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestElementAdjacentFacesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestElementAdjacentFacesCount<ELEM_ID>(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.count<ELEM_ID>(), lva);
 * vcl::elementAdjacentFacesToBuffer<ELEM_ID>(
 *    myMesh, faceAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentFacesSize: size of the largest per-element face
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, FaceMeshConcept MeshType>
void elementAdjacentFacesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentFacesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(mesh.faceCount());

    using IndexType = typename MeshType::IndexType;

    requireFaceCompactness(mesh);
    requireElementComponent<ELEM_ID, CompId::ADJACENT_FACES>(mesh);

    const auto NUM_ROWS = numRows == MeshType::NULL_INDEX ?
                              mesh.template count<ELEM_ID>() :
                              numRows;

    const uint NUM_COLS = largestAdjacentFacesSize;
    const auto SENTINEL = bufferSentinel<BufferValueType>();

    for (IndexType i = 0; const auto& v : mesh.template elements<ELEM_ID>()) {
        uint adjIndex = 0;
        for (auto idx : v.adjFaceIndices()) {
            at(buffer, i, adjIndex, NUM_ROWS, NUM_COLS, storage) =
                idx == MeshType::NULL_INDEX ? SENTINEL :
                                              static_cast<BufferValueType>(idx);
            ++adjIndex;
        }
        // fill the remaining entries with the sentinel value
        for (; adjIndex < NUM_COLS; ++adjIndex) {
            at(buffer, i, adjIndex, NUM_ROWS, NUM_COLS, storage) = SENTINEL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the adjacent faces indices for each vertex
 * of a Mesh. The number of adjacent faces for each vertex can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentFacesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentFacesSize`, or for entries that have no
 * adjacent face (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestVertexAdjacentFacesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestVertexAdjacentFacesCount(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.vertexCount(), lva);
 * vcl::vertexAdjacentFacesToBuffer(
 *    myMesh, faceAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentFacesSize: size of the largest per-vertex face
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void vertexAdjacentFacesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentFacesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementAdjacentFacesToBuffer<ElemId::VERTEX>(
        mesh, buffer, largestAdjacentFacesSize, storage, numRows);
}

/**
 * @brief Export into a buffer the adjacent faces indices for each face
 * of a Mesh. The number of adjacent faces for each face is expected to
 * be equal to the largestFaceSize (see @ref vcl::largestFaceSize).
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.faceCount(), lfs);
 * vcl::faceAdjacentFacesToBuffer(
 *    myMesh, faceAdj.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFacesSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceAdjacentFacesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestFacesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementAdjacentFacesToBuffer<ElemId::FACE>(
        mesh, buffer, largestFacesSize, storage, numRows);
}

/**
 * @brief Export into a buffer the adjacent faces indices for each edge
 * of a Mesh. The number of adjacent faces for each edge can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentFacesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentFacesSize`, or for entries that have no
 * adjacent face (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestEdgeAdjacentFacesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestEdgeAdjacentFacesCount(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.edgeCount(), lva);
 * vcl::edgeAdjacentFacesToBuffer(
 *    myMesh, faceAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentFacesSize: size of the largest per-edge face
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void edgeAdjacentFacesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentFacesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
    requires (EdgeMeshConcept<MeshType>)
{
    elementAdjacentFacesToBuffer<ElemId::EDGE>(
        mesh, buffer, largestAdjacentFacesSize, storage, numRows);
}

/**
 * @brief Export into a buffer the adjacent edges indices for each ELEM_ID
 * element of a Mesh. The number of adjacent edges for each ELEM_ID can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, or for entries that have no
 * adjacent edge (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestElementAdjacentEdgesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestElementAdjacentEdgesCount<ELEM_ID>(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.count<ELEM_ID>(), lva);
 * vcl::elementAdjacentEdgesToBuffer<ELEM_ID>(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest element edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<ElemId ELEM_ID, EdgeMeshConcept MeshType>
void elementAdjacentEdgesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentEdgesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    using namespace detail;
    using BufferValueType = std::remove_cvref_t<decltype(*buffer)>;
    checkBufferIndexCapacity<BufferValueType>(mesh.edgeCount());

    using IndexType = typename MeshType::IndexType;

    requireEdgeCompactness(mesh);
    requireElementComponent<ELEM_ID, CompId::ADJACENT_EDGES>(mesh);

    const auto NUM_ROWS = numRows == MeshType::NULL_INDEX ?
                              mesh.template count<ELEM_ID>() :
                              numRows;

    const uint NUM_COLS = largestAdjacentEdgesSize;
    const auto SENTINEL = bufferSentinel<BufferValueType>();

    for (IndexType i = 0; const auto& v : mesh.template elements<ELEM_ID>()) {
        uint adjIndex = 0;
        for (auto idx : v.adjEdgeIndices()) {
            at(buffer, i, adjIndex, NUM_ROWS, NUM_COLS, storage) =
                idx == MeshType::NULL_INDEX ? SENTINEL :
                                              static_cast<BufferValueType>(idx);
            ++adjIndex;
        }
        // fill the remaining entries with the sentinel value
        for (; adjIndex < NUM_COLS; ++adjIndex) {
            at(buffer, i, adjIndex, NUM_ROWS, NUM_COLS, storage) = SENTINEL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the adjacent edges indices for each vertex
 * of a Mesh. The number of adjacent edges for each vertex can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, or for entries that have no
 * adjacent edge (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestVertexAdjacentEdgesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestVertexAdjacentEdgesCount(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.vertexCount(), lva);
 * vcl::vertexAdjacentEdgesToBuffer(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest per-vertex edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void vertexAdjacentEdgesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentEdgesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementAdjacentEdgesToBuffer<ElemId::VERTEX>(
        mesh, buffer, largestAdjacentEdgesSize, storage, numRows);
}

/**
 * @brief Export into a buffer the adjacent edges indices for each face
 * of a Mesh. The number of adjacent edges for each face can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, or for entries that have no
 * adjacent edge (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestFaceAdjacentEdgesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestFaceAdjacentEdgesCount(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.faceCount(), lva);
 * vcl::faceAdjacentEdgesToBuffer(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest face edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void faceAdjacentEdgesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentEdgesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
    requires (FaceMeshConcept<MeshType>)
{
    elementAdjacentEdgesToBuffer<ElemId::FACE>(
        mesh, buffer, largestAdjacentEdgesSize, storage, numRows);
}

/**
 * @brief Export into a buffer the adjacent edges indices for each edge
 * of a Mesh. The number of adjacent edges for each edge can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, or for entries that have no
 * adjacent edge (e.g. boundary elements), the entries are filled with the
 * sentinel value of the buffer element type (i.e.
 * `static_cast<BufferValueType>(-1)`, which is `-1` for signed integer types,
 * `-1.0` for floating-point types, or the maximum representable value for
 * unsigned integer types).
 *
 * You can use the function @ref vcl::largestEdgeAdjacentEdgesCount to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestEdgeAdjacentEdgesCount(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.edgeCount(), lva);
 * vcl::edgeAdjacentEdgesToBuffer(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest edge edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numRows: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeAdjacentEdgesToBuffer(
    const MeshType&              mesh,
    auto*                        buffer,
    uint                         largestAdjacentEdgesSize,
    MatrixStorageType            storage = MatrixStorageType::ROW_MAJOR,
    typename MeshType::IndexType numRows = MeshType::NULL_INDEX)
{
    elementAdjacentEdgesToBuffer<ElemId::EDGE>(
        mesh, buffer, largestAdjacentEdgesSize, storage, numRows);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
