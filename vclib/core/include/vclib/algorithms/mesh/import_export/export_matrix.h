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
#include <vclib/concepts/space/matrix.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/core/matrix.h>

/**
 * @defgroup export_matrix Export Mesh to Matrix Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Export Mesh to Matrix algorithms.
 *
 * They allow to export mesh data to matrices.
 *
 * You can access these algorithms by including `#include
 * <vclib/algorithms/mesh/import_export.h>`
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
 * size_t argument and an operator(uint).
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
 * Eigen::VectorXi F = vcl::faceIndicesVector<Eigen::VectorXi>(myMesh);
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
Vect faceIndicesVector(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    uint nIndices = countPerFaceVertexReferences(mesh);

    Vect fV(nIndices);

    faceIndicesToBuffer(mesh, fV.data());

    return fV;
}

/**
 * @brief Get a \#F*max(size(F)) Matrix of integers containing the vertex
 * indices for each face of a Mesh.
 *
 * If the mesh is polygonal, the matrix will have a number of rows equal to the
 * greatest polygon of the mesh, and unused values will be set to -1.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi F = vcl::faceMatrix<Eigen::MatrixXi>(myMesh);
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
 * @return \#F*max(size(F)) matrix of vertex indices
 *
 * @ingroup export_matrix
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceIndicesMatrix(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    uint fMaxSize = largestFaceSize(mesh);

    Matrix fM(mesh.faceNumber(), fMaxSize);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    faceIndicesToBuffer(mesh, fM.data(), fMaxSize, stg);

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
 * Eigen::MatrixXi F = vcl::triangulatedFacesMatrix<Eigen::MatrixXi>(myMesh);
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
Matrix triangulatedFaceIndicesMatrix(
    const MeshType&    mesh,
    TriPolyIndexBiMap& indexMap = detail::indexMap)
{
    requireVertexContainerCompactness(mesh);

    uint tNumber = vcl::countTriangulatedTriangles(mesh);

    Matrix tM(tNumber, 3);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    triangulatedFaceIndicesToBuffer(mesh, tM.data(), indexMap, stg, tNumber);

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
 * Eigen::MatrixXi E = vcl::edgeMatrix<Eigen::MatrixXi>(myMesh);
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
Matrix edgeIndicesMatrix(const MeshType& mesh)
{
    requireVertexContainerCompactness(mesh);

    Matrix eM(mesh.edgeNumber(), 2);

    MatrixStorageType stg = matrixStorageType<Matrix>();

    edgeIndicesToBuffer(mesh, eM.data(), stg);

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
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

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
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

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
 * per-vertex colors.
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
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    Vect eQV(mesh.template number<ELEM_ID>());

    elementQualityToBuffer<ELEM_ID>(mesh, eQV.data());

    return eQV;
}

/**
 * @brief Get a \#V Vector of scalars containing the quality of the vertices of
 * a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator(uint), and requires that the mesh has
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
 * size_t argument and an operator(uint), and requires that the mesh has
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

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_MATRIX_H
