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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_MATRIX_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_MATRIX_H

#include "export_buffer.h"

#include <vclib/algorithms/mesh/stat/topology.h>

#include <vclib/mesh.h>
#include <vclib/space/core.h>

/**
 * @defgroup export_matrix Export Mesh to Matrix Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Export Mesh to Matrix algorithms.
 *
 * They allow to export mesh data to matrices.
 */

namespace vcl {

/**
 * @brief Get a \#V*3 Matrix of scalars containing the positions of the
 * vertices of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with an Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d V = vcl::vertexPositionsMatrix<Eigen::MatrixX3d>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#V*3 matrix of scalars (vertex positions)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexPositionsMatrix(const MeshType& mesh)
{
    Matrix vM(mesh.vertexNumber(), 3);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    vertexPositionsToBuffer(mesh, vM.data(), stg);

    return vM;
}

/**
 * @brief Get a \#F Vector of integers containing the sizes of the faces of a
 * Mesh.
 *
 * It could be useful when dealing with polygonal meshes.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi F = vcl::faceSizesVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#F vector of integers (face sizes)
 *
 * @ingroup export_matrix
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceSizesVector(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    Vect fM(mesh.faceNumber());

    faceSizesToBuffer(mesh, fM.data());

    return fM;
}

/**
 * @brief Get a \#(sum of face sizes) Vector of integers containing the vertex
 * indices for each face of a Mesh.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh in
 * the returned vector. Indices are stored consecutively in the vector,
 * following the order the faces appear in the mesh.
 *
 * You can use the function @ref vcl::faceSizesVector to get the sizes of the
 * faces and inspect the vector accordingly. Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi FSizes = vcl::faceSizesVector<Eigen::VectorXi>(myMesh);
 * Eigen::VectorXi F = vcl::faceVertexIndicesVector<Eigen::VectorXi>(myMesh);
 * // read indices for each face
 * uint offset = 0;
 * for (uint i = 0; i < FSizes.size(); ++i) {
 *     uint size = FSizes[i];
 *     for (uint j = 0; j < size; ++j) {
 *         uint vIdx = F[offset + j];
 *         // do something with the vertex index
 *     }
 *     offset += size;
 * }
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @return \#(sum of face sizes) vector of vertex indices
 *
 * @ingroup export_matrix
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceVertexIndicesVector(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    uint nIndices = countPerFaceVertexReferences(mesh);

    Vect fV(nIndices);

    faceVertexIndicesToBuffer(mesh, fV.data());

    return fV;
}

/**
 * @brief Get a \#F*LFS Matrix of integers containing the vertex indices for
 * each face of a Mesh. LFS is the largest face size of the mesh (this number is
 * variable only for polygonal meshes).
 *
 * If the mesh is polygonal, the matrix will have a number of rows equal to the
 * greatest polygon of the mesh, and unused values will be set to -1.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi F = vcl::faceVertexIndicesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#F*LFS matrix of vertex indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceVertexIndicesMatrix(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    uint fMaxSize = largestFaceSize(mesh);

    Matrix fM(mesh.faceNumber(), fMaxSize);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    faceVertexIndicesToBuffer(mesh, fM.data(), fMaxSize, stg);

    return fM;
}

/**
 * @brief Get a \#T*3 Matrix of integers containing the vertex indices for each
 * triangle obtained by the triangulation of the faces of a Mesh.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi F =
 *     vcl::triangulatedFaceVertexIndicesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @param[out] indexMap: map from triangle index to face index
 * @return \#T*3 matrix of vertex indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix triangulatedFaceVertexIndicesMatrix(
    const MeshType&    mesh,
    TriPolyIndexBiMap& indexMap = detail::indexMap)
{
    requireVertexContainerCompactness(mesh);

    uint tNumber = vcl::countTriangulatedTriangles(mesh);

    Matrix tM(tNumber, 3);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    triangulatedFaceVertexIndicesToBuffer(
        mesh, tM.data(), indexMap, stg, tNumber);

    return tM;
}

/**
 * @brief Get a \#E*2 Matrix of integers containing the indices of the vertices
 * of the edges of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi E = vcl::edgeVertexIndicesMatrix<Eigen::MatrixXi>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E*2 matrix of integers (edge indices)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix edgeVertexIndicesMatrix(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    Matrix eM(mesh.edgeNumber(), 2);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    edgeVertexIndicesToBuffer(mesh, eM.data(), stg);

    return eM;
}

/**
 * @brief Get a \#E Vector of booleans (or integers) containing the selection
 * status of the elements identified by `ELEM_ID` of a Mesh. The function is
 * templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S =
 *     vcl::elementSelectionVector<ElemId::VERTEX, Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E vector of booleans or integers (element selection)
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, typename Vect, MeshConcept MeshType>
Vect elementSelectionVector(const MeshType& mesh)
{
    Vect sV(mesh.template number<ELEM_ID>());

    vcl::elementSelectionToBuffer<ELEM_ID>(mesh, sV.data());
    return sV;
}

/**
 * @brief Get a \#V Vector of booleans (or integers) containing the selection
 * status of the vertices of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S = vcl::vertexSelectionVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#V vector of booleans or integers (vertex selection)
 *
 * @ingroup export_matrix
 */
template<typename Vect, MeshConcept MeshType>
Vect vertexSelectionVector(const MeshType& mesh)
{
    return elementSelectionVector<ElemId::VERTEX, Vect>(mesh);
}

/**
 * @brief Get a \#F Vector of booleans (or integers) containing the selection
 * status of the faces of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S = vcl::faceSelectionVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#F vector of booleans or integers (face selection)
 *
 * @ingroup export_matrix
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceSelectionVector(const MeshType& mesh)
{
    return elementSelectionVector<ElemId::FACE, Vect>(mesh);
}

/**
 * @brief Get a \#E Vector of booleans (or integers) containing the selection
 * status of the edges of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S = vcl::edgeSelectionVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E vector of booleans or integers (edge selection)
 *
 * @ingroup export_matrix
 */
template<typename Vect, EdgeMeshConcept MeshType>
Vect edgeSelectionVector(const MeshType& mesh)
{
    return elementSelectionVector<ElemId::EDGE, Vect>(mesh);
}

/**
 * @brief Get a \#E*3 Matrix of scalars containing the normals of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Matrix
 * itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-element normals.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d VN =
 *     vcl::elementNormalsMatrix<ElemId::VERTEX, Eigen::MatrixX3d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * normals available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return \#E*3 matrix of scalars (element normals)
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, MatrixConcept Matrix, MeshConcept MeshType>
Matrix elementNormalsMatrix(const MeshType& mesh)
{
    Matrix eNM(mesh.template number<ELEM_ID>(), 3);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    elementNormalsToBuffer<ELEM_ID>(mesh, eNM.data(), stg);

    return eNM;
}

/**
 * @brief Get a \#V*3 Matrix of scalars containing the normals of the vertices
 * of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-vertex normals.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d VN = vcl::vertexNormalsMatrix<Eigen::MatrixX3d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * normals available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#V*3 matrix of scalars (vertex normals)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexNormalsMatrix(const MeshType& mesh)
{
    return elementNormalsMatrix<ElemId::VERTEX, Matrix>(mesh);
}

/**
 * @brief Get a \#F*3 Matrix of scalars containing the normals of the faces of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-face normals.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d FN = vcl::faceNormalsMatrix<Eigen::MatrixX3d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * normals available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#F*3 matrix of scalars (face normals)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceNormalsMatrix(const MeshType& mesh)
{
    return elementNormalsMatrix<ElemId::FACE, Matrix>(mesh);
}

/**
 * @brief Get a \#E*4 Matrix of integers containing the colors of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Matrix
 * itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-element colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i EC =
 *     vcl::elementColorsMatrix<ElemId::VERTEX, Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return \#E*4 matrix of integers (element colors)
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, MatrixConcept Matrix, MeshConcept MeshType>
Matrix elementColorsMatrix(const MeshType& mesh)
{
    Matrix eCM(mesh.template number<ELEM_ID>(), 4);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    elementColorsToBuffer<ELEM_ID>(mesh, eCM.data(), stg);

    return eCM;
}

/**
 * @brief Get a \#E Vector of integers containing the colors of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Vector
 * itself. The color is packed in a single 32 bit value using the provided
 * format.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-element colors.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi EC =
 *     vcl::elementColorsVector<ElemId::VERTEX, Eigen::VectorXi>(
 *         myMesh, Color::Format::RGBA);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * colors available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return \#E vector of integers (element colors)
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, typename Vect, MeshConcept MeshType>
Vect elementColorsVector(const MeshType& mesh, Color::Format colorFormat)
{
    Vect eCV(mesh.template number<ELEM_ID>());

    elementColorsToBuffer<ELEM_ID>(mesh, eCV.data(), colorFormat);

    return eCV;
}

/**
 * @brief Get a \#V*4 Matrix of integers containing the colors of the vertices
 * of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-vertex colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i VC = vcl::vertexColorsMatrix<Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#V*4 matrix of integers (vertex colors)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexColorsMatrix(const MeshType& mesh)
{
    return elementColorsMatrix<ElemId::VERTEX, Matrix>(mesh);
}

/**
 * @brief Get a \#V Vector of integers containing the colors of the vertices
 * of a Mesh. The function is templated on the Vector itself. The color is
 * packed in a single 32 bit value using the provided format.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-vertex colors.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi VC =
 *     vcl::vertexColorsVector<Eigen::VectorXi>(
 *         myMesh, Color::Format::RGBA);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * colors available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#V vector of integers (vertex colors)
 *
 * @ingroup export_matrix
 */
template<typename Vect, MeshConcept MeshType>
Vect vertexColorsVector(const MeshType& mesh, Color::Format colorFormat)
{
    return elementColorsVector<ElemId::VERTEX, Vect>(mesh, colorFormat);
}

/**
 * @brief Get a \#F*4 Matrix of integers containing the colors of the faces of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-face colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i FC = vcl::faceColorsMatrix<Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#F*4 matrix of integers (face colors)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceColorsMatrix(const MeshType& mesh)
{
    return elementColorsMatrix<ElemId::FACE, Matrix>(mesh);
}

/**
 * @brief Get a \#F Vector of integers containing the colors of the faces
 * of a Mesh. The function is templated on the Vector itself. The color is
 * packed in a single 32 bit value using the provided format.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-face colors.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi FC =
 *     vcl::faceColorsVector<Eigen::VectorXi>(
 *         myMesh, Color::Format::RGBA);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * colors available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#F vector of integers (face colors)
 *
 * @ingroup export_matrix
 */
template<typename Vect, MeshConcept MeshType>
Vect faceColorsVector(const MeshType& mesh, Color::Format colorFormat)
{
    return elementColorsVector<ElemId::FACE, Vect>(mesh, colorFormat);
}

/**
 * @brief Get a \#E*4 Matrix of integers containing the colors of the edges of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-edge colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i EC = vcl::edgeColorsMatrix<Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#E*4 matrix of integers (edge colors)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix edgeColorsMatrix(const MeshType& mesh)
{
    return elementColorsMatrix<ElemId::EDGE, Matrix>(mesh);
}

/**
 * @brief Get a \#E Vector of integers containing the colors of the edges
 * of a Mesh. The function is templated on the Vector itself. The color is
 * packed in a single 32 bit value using the provided format.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-edge colors.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi EC =
 *     vcl::edgeColorsVector<Eigen::VectorXi>(
 *         myMesh, Color::Format::RGBA);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * colors available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#E vector of integers (edge colors)
 *
 * @ingroup export_matrix
 */
template<typename Vect, MeshConcept MeshType>
Vect edgeColorsVector(const MeshType& mesh, Color::Format colorFormat)
{
    return elementColorsVector<ElemId::EDGE, Vect>(mesh, colorFormat);
}

/**
 * @brief Get a \#E Vector of scalars containing the quality of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-element quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd EQ =
 *     vcl::elementQualityVector<ElemId::VERTEX, Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return \#E vector of scalars (element quality)
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, typename Vect, MeshConcept MeshType>
Vect elementQualityVector(const MeshType& mesh)
{
    Vect eQV(mesh.template number<ELEM_ID>());

    elementQualityToBuffer<ELEM_ID>(mesh, eQV.data());

    return eQV;
}

/**
 * @brief Get a \#V Vector of scalars containing the quality of the vertices of
 * a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-vertex quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd VQ = vcl::vertexQualityVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#V vector of scalars (vertex quality)
 *
 * @ingroup export_matrix
 */
template<typename Vect, MeshConcept MeshType>
Vect vertexQualityVector(const MeshType& mesh)
{
    return elementQualityVector<ElemId::VERTEX, Vect>(mesh);
}

/**
 * @brief Get a \#F Vector of scalars containing the quality of the faces of
 * a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-face quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd FQ = vcl::faceQualityVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#F vector of scalars (face quality)
 *
 * @ingroup export_matrix
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceQualityVector(const MeshType& mesh)
{
    return elementQualityVector<ElemId::FACE, Vect>(mesh);
}

/**
 * @brief Get a \#E Vector of scalars containing the quality of the edges of
 * a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-edge quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd EQ = vcl::edgeQualityVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#E vector of scalars (edge quality)
 *
 * @ingroup export_matrix
 */
template<typename Vect, EdgeMeshConcept MeshType>
Vect edgeQualityVector(const MeshType& mesh)
{
    return elementQualityVector<ElemId::EDGE, Vect>(mesh);
}

/**
 * @brief Get a \#V*2 Matrix of scalars containing the texcoords of the vertices
 * of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-vertex texcoords.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX2d VT = vcl::vertexTexCoordsMatrix<Eigen::MatrixX2d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * texcoords available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#V*2 matrix of scalars (vertex texcoords)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexTexCoordsMatrix(const MeshType& mesh)
{
    Matrix vTCM(mesh.vertexNumber(), 2);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    vertexTexCoordsToBuffer(mesh, vTCM.data(), stg);

    return vTCM;
}

/**
 * @brief Get a \#V vector of scalars containing the material indices of the
 * vertices of a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-vertex material indices.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd VTI =
 *     vcl::vertexMaterialIndicesVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * material indices available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#V vector of scalars (vertex material indices)
 *
 * @ingroup export_matrix
 */
template<typename Vect, MeshConcept MeshType>
Vect vertexMaterialIndicesVector(const MeshType& mesh)
{
    Vect vTCI(mesh.vertexNumber());

    vertexMaterialIndicesToBuffer(mesh, vTCI.data());

    return vTCI;
}

/**
 * @brief Get a \#F*(LFS*2) Matrix of scalars containing the wedge texcoords of
 * the faces of a Mesh. The function is templated on the Matrix itself. LFS is
 * the largest face size of the mesh (this number is variable only for polygonal
 * meshes).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-face wedge texcoords.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXd FWT = vcl::faceWedgeTexCoordsMatrix<Eigen::MatrixXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * wedge texcoords available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#F*(LFS*2) matrix of scalars (face wedge texcoords)
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceWedgeTexCoordsMatrix(const MeshType& mesh)
{
    uint lfs = vcl::largestFaceSize(mesh);

    Matrix fTCM(mesh.faceNumber(), lfs * 2);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    faceWedgeTexCoordsToBuffer(mesh, fTCM.data(), lfs, stg);

    return fTCM;
}

/**
 * @brief Get a \#F vector of scalars containing the wedge texcoord indices of
 * the faces of a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-face texcoords.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd VTI =
 *     vcl::faceWedgeTexCoordIndicesVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * wedge texcoords available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return \#F vector of scalars (face wedge texcoord indices)
 *
 * @ingroup export_matrix
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceWedgeTexCoordIndicesVector(const MeshType& mesh)
{
    Vect fTCI(mesh.faceNumber());

    faceWedgeTexCoordIndicesToBuffer(mesh, fTCI.data());

    return fTCI;
}

/**
 * @brief Get a \#V Container of Containers of integers (T) containing the
 * adjacent vertex indices for each vertex of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::vertexAdjacentVerticesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-vertex AdjacentVertices component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent vertices available.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent vertex indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    MeshConcept MeshType>
Container<Container<T>> vertexAdjacentVerticesVectors(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);
    requirePerVertexAdjacentVertices(mesh);

    Container<Container<T>> vv(mesh.vertexNumber());

    auto vvIt = vv.begin();
    for (const auto& v : mesh.vertices()) {
        auto& vec = *vvIt;

        vec.resize(v.adjVerticesNumber());
        auto vecIt = vec.begin();
        for (const auto* ve : v.adjVertices()) {
            uint idx = ve ? ve->index() : UINT_NULL;
            *vecIt   = T(idx);
            ++vecIt;
        }

        ++vvIt;
    }

    return vv;
}

/**
 * @brief Get a \#V*LVA Matrix of integers containing the adjacent vertex
 * indices for each vertex of a Mesh. LVA is the largest vertex adjacency size
 * of the mesh. The unused values will be set to -1 (@ref vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi VA =
 *     vcl::vertexAdjacentVerticesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent vertices available.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#V*LVA matrix of adjacent vertex indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexAdjacentVerticesMatrix(const MeshType& mesh)
{
    uint lva = vcl::largestPerVertexAdjacentVerticesNumber(mesh);

    Matrix vAVM(mesh.vertexNumber(), lva);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    vertexAdjacentVerticesToBuffer(mesh, vAVM.data(), lva, stg);

    return vAVM;
}

/**
 * @brief Get a \#E Container of Containers of integers (T) containing the
 * adjacent face indices for each ELEM_ID element of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::elementAdjacentFacesVectors<
 *         ElemID::VERTEX, std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-element AdjacentFaces component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent faces available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element ELEM_ID indices of the mesh. This scenario is
 * possible when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the ELEM_ID element container before calling this
 * function.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<
    uint ELEM_ID,
    template<typename, typename...>
    typename Container,
    typename T,
    FaceMeshConcept MeshType>
Container<Container<T>> elementAdjacentFacesVectors(const MeshType& mesh)
{
    requireFaceContainerCompactness(mesh);
    requirePerElementComponent<ELEM_ID, CompId::ADJACENT_FACES>(mesh);

    Container<Container<T>> vv(mesh.template number<ELEM_ID>());

    auto vvIt = vv.begin();
    for (const auto& v : mesh.template elements<ELEM_ID>()) {
        auto& vec = *vvIt;

        vec.resize(v.adjFacesNumber());
        auto vecIt = vec.begin();
        for (const auto* fe : v.adjFaces()) {
            uint idx = fe ? fe->index() : UINT_NULL;
            *vecIt   = T(idx);
            ++vecIt;
        }

        ++vvIt;
    }

    return vv;
}

/**
 * @brief Get a \#E*LFA Matrix of integers containing the adjacent face
 * indices for each ELEM_ID element of a Mesh. LFA is the largest face
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi FA =
 *     vcl::elementAdjacentFacesMatrix<ElemId::VERTEX, Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent faces available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element ELEM_ID indices of the mesh. This scenario is
 * possible when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the ELEM_ID element container before calling this
 * function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E*LFA matrix of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix elementAdjacentFacesMatrix(const MeshType& mesh)
{
    uint lfa = vcl::largestPerElementAdjacentFacesNumber<ELEM_ID>(mesh);

    Matrix eAFM(mesh.template number<ELEM_ID>(), lfa);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    elementAdjacentFacesToBuffer<ELEM_ID>(mesh, eAFM.data(), lfa, stg);

    return eAFM;
}

/**
 * @brief Get a \#V Container of Containers of integers (T) containing the
 * adjacent face indices for each vertex of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::vertexAdjacentFacesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-vertex AdjacentFaces component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent faces available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    FaceMeshConcept MeshType>
Container<Container<T>> vertexAdjacentFacesVectors(const MeshType& mesh)
{
    return elementAdjacentFacesVectors<ElemId::VERTEX, Container, T>(mesh);
}

/**
 * @brief Get a \#V*LFA Matrix of integers containing the adjacent face
 * indices for each vertex of a Mesh. LFA is the largest face
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi FA =
 *     vcl::vertexAdjacentFacesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent faces available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#V*LFA matrix of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix vertexAdjacentFacesMatrix(const MeshType& mesh)
{
    return elementAdjacentFacesMatrix<ElemId::VERTEX, Matrix>(mesh);
}

/**
 * @brief Get a \#F Container of Containers of integers (T) containing the
 * adjacent face indices for each face of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::faceAdjacentFacesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-face AdjacentFaces component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent faces available.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    FaceMeshConcept MeshType>
Container<Container<T>> faceAdjacentFacesVectors(const MeshType& mesh)
{
    return elementAdjacentFacesVectors<ElemId::FACE, Container, T>(mesh);
}

/**
 * @brief Get a \#F*LFA Matrix of integers containing the adjacent face
 * indices for each face of a Mesh. LFA is the largest face
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi FA =
 *     vcl::faceAdjacentFacesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent faces available.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#F*LFA matrix of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceAdjacentFacesMatrix(const MeshType& mesh)
{
    return elementAdjacentFacesMatrix<ElemId::FACE, Matrix>(mesh);
}

/**
 * @brief Get a \#E Container of Containers of integers (T) containing the
 * adjacent face indices for each edge of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::edgeAdjacentFacesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-edge AdjacentFaces component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * adjacent faces available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept and EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    FaceMeshConcept MeshType>
Container<Container<T>> edgeAdjacentFacesVectors(const MeshType& mesh)
    requires EdgeMeshConcept<MeshType>
{
    return elementAdjacentFacesVectors<ElemId::EDGE, Container, T>(mesh);
}

/**
 * @brief Get a \#E*LFA Matrix of integers containing the adjacent face
 * indices for each edge of a Mesh. LFA is the largest face
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi FA =
 *     vcl::edgeAdjacentFacesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the face container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * adjacent faces available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept and EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E*LFA matrix of adjacent face indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix edgeAdjacentFacesMatrix(const MeshType& mesh)
    requires EdgeMeshConcept<MeshType>
{
    return elementAdjacentFacesMatrix<ElemId::EDGE, Matrix>(mesh);
}

/**
 * @brief Get a \#E Container of Containers of integers (T) containing the
 * adjacent edge indices for each ELEM_ID element of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::elementAdjacentEdgesVectors<
 *         ElemID::VERTEX, std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-element AdjacentEdges component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent edges available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element ELEM_ID indices of the mesh. This scenario is
 * possible when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the ELEM_ID element container before calling this
 * function.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<
    uint ELEM_ID,
    template<typename, typename...>
    typename Container,
    typename T,
    EdgeMeshConcept MeshType>
Container<Container<T>> elementAdjacentEdgesVectors(const MeshType& mesh)
{
    requireEdgeContainerCompactness(mesh);
    requirePerElementComponent<ELEM_ID, CompId::ADJACENT_EDGES>(mesh);

    Container<Container<T>> vv(mesh.template number<ELEM_ID>());

    auto vvIt = vv.begin();
    for (const auto& v : mesh.template elements<ELEM_ID>()) {
        auto& vec = *vvIt;

        vec.resize(v.adjEdgesNumber());
        auto vecIt = vec.begin();
        for (const auto* fe : v.adjEdges()) {
            uint idx = fe ? fe->index() : UINT_NULL;
            *vecIt   = T(idx);
            ++vecIt;
        }

        ++vvIt;
    }

    return vv;
}

/**
 * @brief Get a \#E*LEA Matrix of integers containing the adjacent edge
 * indices for each ELEM_ID element of a Mesh. LEA is the largest edge
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi EA =
 *     vcl::elementAdjacentEdgesMatrix<ElemId::VERTEX, Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * adjacent edges available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element ELEM_ID indices of the mesh. This scenario is
 * possible when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the ELEM_ID element container before calling this
 * function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E*LEA matrix of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<uint ELEM_ID, MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix elementAdjacentEdgesMatrix(const MeshType& mesh)
{
    uint lea = vcl::largestPerElementAdjacentEdgesNumber<ELEM_ID>(mesh);

    Matrix eAEM(mesh.template number<ELEM_ID>(), lea);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    elementAdjacentEdgesToBuffer<ELEM_ID>(mesh, eAEM.data(), lea, stg);

    return eAEM;
}

/**
 * @brief Get a \#V Container of Containers of integers (T) containing the
 * adjacent edge indices for each vertex of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::vertexAdjacentEdgesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-vertex AdjacentEdges component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent edges available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    EdgeMeshConcept MeshType>
Container<Container<T>> vertexAdjacentEdgesVectors(const MeshType& mesh)
{
    return elementAdjacentEdgesVectors<ElemId::VERTEX, Container, T>(mesh);
}

/**
 * @brief Get a \#V*LEA Matrix of integers containing the adjacent edge
 * indices for each vertex of a Mesh. LEA is the largest edge
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi EA =
 *     vcl::vertexAdjacentEdgesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * adjacent edges available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#V*LEA matrix of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix vertexAdjacentEdgesMatrix(const MeshType& mesh)
{
    return elementAdjacentEdgesMatrix<ElemId::VERTEX, Matrix>(mesh);
}

/**
 * @brief Get a \#F Container of Containers of integers (T) containing the
 * adjacent edge indices for each face of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::faceAdjacentEdgesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-face AdjacentEdges component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent edges available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept and EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    EdgeMeshConcept MeshType>
Container<Container<T>> faceAdjacentEdgesVectors(const MeshType& mesh)
    requires FaceMeshConcept<MeshType>
{
    return elementAdjacentEdgesVectors<ElemId::FACE, Container, T>(mesh);
}

/**
 * @brief Get a \#F*LEA Matrix of integers containing the adjacent edge
 * indices for each face of a Mesh. LEA is the largest edge
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi EA =
 *     vcl::faceAdjacentEdgesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * adjacent edges available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept and EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#F*LEA matrix of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix faceAdjacentEdgesMatrix(const MeshType& mesh)
    requires FaceMeshConcept<MeshType>
{
    return elementAdjacentEdgesMatrix<ElemId::FACE, Matrix>(mesh);
}

/**
 * @brief Get a \#E Container of Containers of integers (T) containing the
 * adjacent edge indices for each edge of a Mesh.
 *
 * This function works with every Container type that is iterable and has a
 * resize member function.
 *
 * Usage example with std::vector:
 *
 * @code{.cpp}
 * std::vector<std::vector<vcl::uint>> =
 *     vcl::edgeAdjacentEdgesVectors<std::vector, vcl::uint>(myMesh);
 * @endif
 *
 * @note The per-edge AdjacentEdges component must be computed before
 * calling this function.
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * adjacent edges available.
 *
 * @tparam Container: type of the container to be returned.
 * @tparam T: type of the integers to be stored in the containers.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return Container<Container<T>> of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<
    template<typename, typename...>
    typename Container,
    typename T,
    EdgeMeshConcept MeshType>
Container<Container<T>> edgeAdjacentEdgesVectors(const MeshType& mesh)
{
    return elementAdjacentEdgesVectors<ElemId::EDGE, Container, T>(mesh);
}

/**
 * @brief Get a \#E*LEA Matrix of integers containing the adjacent edge
 * indices for each edge of a Mesh. LEA is the largest edge
 * adjacency size of the mesh. The unused values will be set to -1 (@ref
 * vcl::UINT_NULL).
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi EA =
 *     vcl::edgeAdjacentEdgesMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the edge container is not
 * compact.
 * @throws vcl::MissingComponentException if the mesh does not have per-edge
 * adjacent edges available.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return \#E*LEA matrix of adjacent edge indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix edgeAdjacentEdgesMatrix(const MeshType& mesh)
{
    return elementAdjacentEdgesMatrix<ElemId::EDGE, Matrix>(mesh);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_MATRIX_H
