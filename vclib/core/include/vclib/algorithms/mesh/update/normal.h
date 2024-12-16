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

#ifndef VCL_ALGORITHMS_MESH_UPDATE_NORMAL_H
#define VCL_ALGORITHMS_MESH_UPDATE_NORMAL_H

#include <vclib/algorithms/core/polygon.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/parallel.h>
#include <vclib/space/core/matrix.h>

namespace vcl {

namespace detail {

template<uint ELEM_ID, LoggerConcept LogType = NullLogger>
void normalizeNoThrow(auto& elem, LogType& log = nullLogger)
{
    try {
        elem.normal().normalize();
    }
    catch (const std::exception& e) {
        log.log(
            log.WARNING_LOG,
            elementEnumString<ELEM_ID>() + " " + std::to_string(elem.index()) +
                ": " + e.what());
    }
}

} // namespace detail

/**
 * @brief Sets to zero the normals of all the `ELEM_ID` elements of the mesh,
 * including the unreferenced ones.
 *
 * Requirements:
 * - Mesh:
 *   - `ELEM_ID`:
 *     - Normal
 *
 * @tparam ELEM_ID: The ID of an Element, that is a value in the ElementIDEnum.
 *
 * @param[in,out] mesh: The mesh on which clear the `ELEM_ID` normals.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<uint ELEM_ID, LoggerConcept LogType = NullLogger>
void clearPerElementNormals(MeshConcept auto& mesh, LogType& log = nullLogger)
{
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    log.log(0, "Clearing per-" + elementEnumString<ELEM_ID>() + " normals...");

    parallelFor(mesh.template elements<ELEM_ID>(), [](auto& e) {
        e.normal().setZero();
    });

    log.log(100, "Per-" + elementEnumString<ELEM_ID>() + " normals cleared.");
}

/**
 * @brief Normalizes the length of the normals of all the `ELEM_ID` elements.
 *
 * Requirements:
 * - Mesh:
 *   - `ELEM_ID`:
 *     - Normal
 *
 * @tparam ELEM_ID: The ID of an Element, that is a value in the ElementIDEnum.
 *
 * @param[in,out] mesh: the mesh on which normalize the `ELEM_ID` normals.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<uint ELEM_ID, LoggerConcept LogType = NullLogger>
void normalizePerElementNormals(
    MeshConcept auto& mesh,
    LogType&          log = nullLogger)
{
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    log.log(
        0, "Normalizing per-" + elementEnumString<ELEM_ID>() + " normals...");

    // define a lambda that, for each element, normalizes the normal
    auto normalize = [&](auto& elem) {
        detail::normalizeNoThrow<ELEM_ID>(elem, log);
    };

    parallelFor(mesh.template elements<ELEM_ID>(), normalize);

    log.log(
        100, "Per-" + elementEnumString<ELEM_ID>() + " normals normalized.");
}

/**
 * @brief Multiplies the normals of all the `ELEM_ID` elements by the given 3x3
 * Matrix.
 *
 * If removeScalingFromMatrix is true (default), the scale component is
 * removed from the matrix.
 *
 * Requirements:
 * - Mesh:
 *   - `ELEM_ID`:
 *     - Normal
 *
 * @param[in,out] mesh: the mesh on which multiply the element normals.
 * @param[in] mat: the 3x3 matrix that is multiplied to the normals.
 * @param[in] removeScalingFromMatrix: if true (default), the scale component is
 * removed from the matrix.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<uint ELEM_ID, typename MScalar, LoggerConcept LogType = NullLogger>
void multiplyPerElementNormalsByMatrix(
    MeshConcept auto& mesh,
    Matrix33<MScalar> mat,
    bool              removeScalingFromMatrix = true,
    LogType&          log                     = nullLogger)
{
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    if (removeScalingFromMatrix) {
        MScalar scaleX = std::sqrt(
            mat(0, 0) * mat(0, 0) + mat(0, 1) * mat(0, 1) +
            mat(0, 2) * mat(0, 2));
        MScalar scaleY = std::sqrt(
            mat(1, 0) * mat(1, 0) + mat(1, 1) * mat(1, 1) +
            mat(1, 2) * mat(1, 2));
        MScalar scaleZ = std::sqrt(
            mat(2, 0) * mat(2, 0) + mat(2, 1) * mat(2, 1) +
            mat(2, 2) * mat(2, 2));
        for (int i = 0; i < 3; ++i) {
            mat(0, i) /= scaleX;
            mat(1, i) /= scaleY;
            mat(2, i) /= scaleZ;
        }
    }

    log.log(
        0,
        "Multiplying per-" + elementEnumString<ELEM_ID>() +
            " normals by matrix...");

    parallelFor(mesh.template elements<ELEM_ID>(), [&](auto& e) {
        e.normal() = mat * e.normal();
    });

    log.log(
        100, "Per-" + elementEnumString<ELEM_ID>() + " normals multiplied.");
}

/**
 * @brief Multiplies the normals of all the `ELEM_ID` elements by the given TRS
 * 4x4 Matrix.
 *
 * The normals are multiplied by the 3x3 rotation matrix of the given TRS
 * matrix. If removeScalingFromMatrix is true (default), the scale component is
 * removed from the matrix.
 *
 * Requirements:
 * - Mesh:
 *   - `ELEM_ID`:
 *     - Normal
 *
 * @param[in,out] mesh: the mesh on which multiply the element normals.
 * @param[in] mat: the 4x4 TRS matrix that is multiplied to the normals.
 * @param[in] removeScalingFromMatrix: if true (default), the scale component is
 * removed from the matrix.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<uint ELEM_ID, typename MScalar, LoggerConcept LogType = NullLogger>
void multiplyPerElementNormalsByMatrix(
    MeshConcept auto&        mesh,
    const Matrix44<MScalar>& mat,
    bool                     removeScalingFromMatrix = true,
    LogType&                 log                     = nullLogger)
{
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    Matrix33<MScalar> m33 = mat.block(0, 0, 3, 3);
    multiplyPerElementNormalsByMatrix<ELEM_ID>(
        mesh, m33, removeScalingFromMatrix, log);
}

/**
 * @brief Same as clearPerElementNormals, but for the vertex normals.
 *
 * @see clearPerElementNormals
 */
template<LoggerConcept LogType = NullLogger>
void clearPerVertexNormals(MeshConcept auto& mesh, LogType& log = nullLogger)
{
    clearPerElementNormals<ElemId::VERTEX>(mesh, log);
}

/**
 * @brief Sets to zero all the normals of vertices that are referenced by at
 * least one element, leaving unchanged all the normals of the unreferenced
 * vertices that may be still useful.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @note Only VertexReferences (VertexPointers or VertexIndices) component is
 * checked for each element of the mesh. AdjacentVertices component or other
 * componens that store vertex references are not considered.
 *
 * @param[in,out] mesh: The mesh on which clear the referenced vertex normals.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void clearPerReferencedVertexNormals(MeshType& mesh, LogType& log = nullLogger)
{
    requirePerVertexNormal(mesh);

    // function that is called for each container Cont of the mesh
    auto f = [&]<mesh::ElementContainerConcept Cont>() {
        using Elem = typename Cont::ElementType; // the Element type of Cont
        if constexpr (comp::HasVertexReferences<Elem>) { // if Elem has vertices
            for (auto& e : mesh.template elements<Elem::ELEMENT_ID>()) {
                for (auto* v : e.vertices()) {
                    v->normal().setZero();
                }
            }
        }
    };

    log.log(0, "Clearing per-Vertex normals...");

    // apply the function f to all the containers of the mesh
    ForEachType<typename MeshType::Containers>::apply(f);

    log.log(100, "Per-Vertex normals cleared.");
}

/**
 * @brief Same as clearPerElementNormals, but for the face normals.
 *
 * @see clearPerElementNormals
 */
template<LoggerConcept LogType = NullLogger>
void clearPerFaceNormals(FaceMeshConcept auto& mesh, LogType& log = nullLogger)
{
    clearPerElementNormals<ElemId::FACE>(mesh, log);
}

/**
 * @brief Same as normalizePerElementNormals, but for the vertex normals.
 *
 * @see normalizePerElementNormals
 */
template<LoggerConcept LogType = NullLogger>
void normalizePerVertexNormals(
    MeshConcept auto& mesh,
    LogType&          log = nullLogger)
{
    normalizePerElementNormals<ElemId::VERTEX>(mesh, log);
}

/**
 * @brief Normalizes the length of normals the referenced vertices.
 *
 * Requirements:
 * - Mesh:
 *   - Vertex:
 *     - Normal
 *   - Face
 *
 * @note Only VertexReferences (VertexPointers or VertexIndices) component is
 * checked for each element of the mesh. AdjacentVertices component or other
 * componens that store vertex references are not considered.
 *
 * @param[in,out] mesh: the mesh on which normalize the vertex normals.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void normalizePerReferencedVertexNormals(
    MeshType& mesh,
    LogType&  log = nullLogger)
{
    requirePerVertexNormal(mesh);

    // function that is called for each container Cont
    auto f = [&]<mesh::ElementContainerConcept Cont>() {
        using Elem = typename Cont::ElementType; // the Element type of Cont
        if constexpr (comp::HasVertexReferences<Elem>) { // if Elem has vertices
            for (auto& e : mesh.template elements<Elem::ELEMENT_ID>()) {
                for (auto* v : e.vertices()) {
                    detail::normalizeNoThrow<ElemId::VERTEX>(*v, log);
                }
            }
        }
    };

    log.log(0, "Normalizing per-Vertex normals...");

    // apply the function f to all the containers of the mesh
    ForEachType<typename MeshType::Containers>::apply(f);

    log.log(100, "Per-Vertex normals normalized.");
}

/**
 * @brief Same as normalizePerElementNormals, but for the face normals.
 *
 * @see normalizePerElementNormals
 */
template<LoggerConcept LogType = NullLogger>
void normalizePerFaceNormals(
    FaceMeshConcept auto& mesh,
    LogType&              log = nullLogger)
{
    normalizePerElementNormals<ElemId::FACE>(mesh, log);
}

/**
 * @brief Same as multiplyPerElementNormalsByMatrix, but for the vertex
 * normals.
 *
 * Accepts both 3x3 and 4x4 matrices.
 *
 * @see multiplyPerElementNormalsByMatrix
 */
template<typename Matrix, LoggerConcept LogType = NullLogger>
void multiplyPerVertexNormalsByMatrix(
    MeshConcept auto& mesh,
    const Matrix&     mat,
    bool              removeScalingFromMatrix = true,
    LogType&          log                     = nullLogger)
{
    multiplyPerElementNormalsByMatrix<ElemId::VERTEX>(
        mesh, mat, removeScalingFromMatrix, log);
}

/**
 * @brief Same as multiplyPerElementNormalsByMatrix, but for the face normals.
 *
 * Accepts both 3x3 and 4x4 matrices.
 *
 * @see multiplyPerElementNormalsByMatrix
 */
template<typename Matrix, LoggerConcept LogType = NullLogger>
void multiplyPerFaceNormalsByMatrix(
    FaceMeshConcept auto& mesh,
    const Matrix&         mat,
    bool                  removeScalingFromMatrix = true,
    LogType&              log                     = nullLogger)
{
    multiplyPerElementNormalsByMatrix<ElemId::FACE>(
        mesh, mat, removeScalingFromMatrix, log);
}

/**
 * @brief Computes and sets the face normal.
 *
 * The function works both for Triangle and Polygonal faces.
 *
 * For polygonal faces, the normal is computed as the normalized sum of the
 * cross products of each triplet of consecutive vertices of the face.
 *
 * @see vcl::Polygon::normal()
 *
 * @param[in,out] mesh: the mesh on which compute the face normals.
 * @param[in] normalize: if true (default), normals are normalized after
 * computation.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<LoggerConcept LogType = NullLogger>
void updatePerFaceNormals(
    FaceMeshConcept auto& mesh,
    bool                  normalize = true,
    LogType&              log       = nullLogger)
{
    requirePerFaceNormal(mesh);

    using FaceType   = RemoveRef<decltype(mesh)>::FaceType;
    using ScalarType = FaceType::NormalType::ScalarType;

    log.log(0, "Updating per-Face normals...");

    parallelFor(mesh.faces(), [](auto& f) {
        f.normal() = faceNormal(f).template cast<ScalarType>();
    });

    if (normalize) {
        log.startNewTask(50, 100, "Normalizing per-Face normals...");
        normalizePerFaceNormals(mesh, log);
        log.endTask("Normalizing per-Face normals...");
    }

    log.log(100, "Per-Face normals updated.");
}

/**
 * @brief Computes the vertex normal as the classic area weighted average.
 *
 * This function does not need or exploit current face normals. Unreferenced
 * vertex normals are left unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in,out] mesh: the mesh on which compute the vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after
 * computation.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<LoggerConcept LogType = NullLogger>
void updatePerVertexNormals(
    FaceMeshConcept auto& mesh,
    bool                  normalize = true,
    LogType&              log       = nullLogger)
{
    using VertexType = RemoveRef<decltype(mesh)>::VertexType;
    using NScalar    = VertexType::NormalType::ScalarType;

    log.log(0, "Updating per-Vertex normals...");

    log.startNewTask(0, 20, "Clearing per-Vertex normals...");
    clearPerReferencedVertexNormals(mesh, log);
    log.endTask("Clearing per-Vertex normals...");

    log.log(20, "Updating per-Vertex normals...");

    for (auto& f : mesh.faces()) {
        for (auto* v : f.vertices()) {
            v->normal() += faceNormal(f).template cast<NScalar>();
        }
    }

    if (normalize) {
        log.startNewTask(80, 100, "Normalizing per-Vertex normals...");
        normalizePerReferencedVertexNormals(mesh, log);
        log.endTask("Normalizing per-Vertex normals...");
    }

    log.log(100, "Per-Vertex normals updated.");
}

/**
 * @brief Computes the vertex normal as the sum of the adjacent faces normals.
 *
 * Unreferenced vertex normals are left unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *     - Normal
 *
 * @param[in,out] mesh: the mesh on which compute the vertex normals.
 * @param[in] normalize: if true (default), normals are normalized after
 * computation.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<LoggerConcept LogType = NullLogger>
void updatePerVertexNormalsFromFaceNormals(
    FaceMeshConcept auto& mesh,
    bool                  normalize = true,
    LogType&              log       = nullLogger)
{
    requirePerFaceNormal(mesh);

    using VertexType = RemoveRef<decltype(mesh)>::VertexType;
    using ScalarType = VertexType::NormalType::ScalarType;

    log.log(0, "Updating per-Vertex normals...");

    log.startNewTask(0, 20, "Clearing per-Vertex normals...");
    clearPerReferencedVertexNormals(mesh, log);
    log.endTask("Clearing per-Vertex normals...");

    log.log(20, "Updating per-Vertex normals...");

    for (auto& f : mesh.faces()) {
        for (auto* v : f.vertices()) {
            v->normal() += f.normal().template cast<ScalarType>();
        }
    }

    if (normalize) {
        log.startNewTask(80, 100, "Normalizing per-Vertex normals...");
        normalizePerReferencedVertexNormals(mesh, log);
        log.endTask("Normalizing per-Vertex normals...");
    }

    log.log(100, "Per-Vertex normals updated.");
}

/**
 * @brief Computes the face normals and then vertex normal as the angle weighted
 * average.
 *
 * The result is the same as calling updatePerFaceNormals() and then
 * updatePerVertexNormals(), but it is more efficient because it exploits the
 * (not yet normalized) face normals to compute the vertex normals.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *     - Normal
 *
 * @param[in,out] mesh: the mesh on which compute the normals.
 * @param[in] normalize: if true (default), normals are normalized after
 * computation.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<LoggerConcept LogType = NullLogger>
void updatePerVertexAndFaceNormals(
    FaceMeshConcept auto& mesh,
    bool                  normalize = true,
    LogType&              log       = nullLogger)
{
    log.log(0, "Updating per-Vertex and per-Face normals...");

    log.startNewTask(0, 40, "Updating per-Face normals...");
    updatePerFaceNormals(mesh, false, log); // normals are not normalized here
    log.endTask("");

    log.startNewTask(40, 80, "Updating per-Vertex normals...");
    updatePerVertexNormalsFromFaceNormals(mesh, normalize, log);
    log.endTask("");

    if (normalize) {
        log.startNewTask(80, 100, "Normalizing per-Face normals...");
        normalizePerFaceNormals(mesh, log);
        log.endTask("");
    }

    log.log(100, "Per-Vertex normals updated.");
}

/**
 * @brief Computes the vertex normal as an angle weighted average.
 *
 * The normal of a vertex `v` computed as a weighted sum the incident face
 * normals. The weight is simlply the angle of the involved wedge. Described in:
 *
 * ```
 * G. Thurmer, C. A. Wuthrich
 *   "Computing vertex normals from polygonal facets"
 *   Journal of Graphics Tools, 1998
 * ```
 *
 * This function does not need or exploit current face normals. Unreferenced
 * vertex normals are left unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in,out] mesh: the mesh on which compute the angle weighted vertex
 * normals.
 * @param[in] normalize: if true (default), normals are normalized after
 * computation.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<LoggerConcept LogType = NullLogger>
void updatePerVertexNormalsAngleWeighted(
    FaceMeshConcept auto& mesh,
    bool                  normalize = true,
    LogType&              log       = nullLogger)
{
    using VertexType  = RemoveRef<decltype(mesh)>::VertexType;
    using NormalType  = VertexType::NormalType;
    using NScalarType = NormalType::ScalarType;

    log.log(0, "Updating per-Vertex normals...");

    log.startNewTask(0, 5, "Clearing per-Vertex normals...");
    clearPerReferencedVertexNormals(mesh, log);
    log.endTask("Clearing per-Vertex normals...");

    log.log(5, "Updating per-Vertex normals...");

    for (auto& f : mesh.faces()) {
        auto n = faceNormal(f).template cast<NScalarType>();

        for (uint i = 0; i < f.vertexNumber(); ++i) {
            NormalType vec1 =
                (f.vertexMod(i - 1)->coord() - f.vertexMod(i)->coord())
                    .normalized()
                    .template cast<NScalarType>();
            NormalType vec2 =
                (f.vertexMod(i + 1)->coord() - f.vertexMod(i)->coord())
                    .normalized()
                    .template cast<NScalarType>();

            f.vertex(i)->normal() += n * vec1.angle(vec2);
        }
    }

    if (normalize) {
        log.startNewTask(95, 100, "Normalizing per-Vertex normals...");
        normalizePerReferencedVertexNormals(mesh, log);
        log.endTask("Normalizing per-Vertex normals...");
    }

    log.log(100, "Per-Vertex normals updated.");
}

/**
 * @brief Computes the vertex normal using the Max et al. weighting scheme.
 *
 * The normal of a vertex v is computed according to the formula described in:
 *
 * ```
 * Max, N.,
 *   "Weights for Computing Vertex Normals from Facet Normals",
 *   Journal of Graphics Tools, 4(2) (1999)
 * ```
 *
 * The weight for each wedge is the cross product of the two edge over the
 * product of the square of the two edge lengths. According to the original
 * paper it is perfect only for spherical surface, but it should perform well...
 *
 * This function does not need or exploit current face normals. Unreferenced
 * vertex normals are left unchanged.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *   - Faces
 *
 * @param[in,out] mesh: the mesh on which compute the Max et al. weighted vertex
 * normals.
 * @param[in] normalize: if true (default), normals are normalized after
 * computation.
 * @param[in,out] log: The logger used to log the performed operations.
 */
template<LoggerConcept LogType = NullLogger>
void updatePerVertexNormalsNelsonMaxWeighted(
    FaceMeshConcept auto& mesh,
    bool                  normalize = true,
    LogType&              log       = nullLogger)
{
    using VertexType  = RemoveRef<decltype(mesh)>::VertexType;
    using NScalarType = VertexType::NormalType::ScalarType;

    log.log(0, "Updating per-Vertex normals...");

    log.startNewTask(0, 5, "Clearing per-Vertex normals...");
    clearPerReferencedVertexNormals(mesh, log);
    log.endTask("Clearing per-Vertex normals...");

    log.log(5, "Updating per-Vertex normals...");

    for (auto& f : mesh.faces()) {
        auto n = faceNormal(f).template cast<NScalarType>();

        for (uint i = 0; i < f.vertexNumber(); ++i) {
            NScalarType e1 =
                (f.vertexMod(i - 1)->coord() - f.vertexMod(i)->coord())
                    .squaredNorm();
            NScalarType e2 =
                (f.vertexMod(i + 1)->coord() - f.vertexMod(i)->coord())
                    .squaredNorm();

            f.vertex(i)->normal() += n / (e1 * e2);
        }
    }

    if (normalize) {
        log.startNewTask(95, 100, "Normalizing per-Vertex normals...");
        normalizePerReferencedVertexNormals(mesh, log);
        log.endTask("Normalizing per-Vertex normals...");
    }

    log.log(100, "Per-Vertex normals updated.");
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_NORMAL_H
