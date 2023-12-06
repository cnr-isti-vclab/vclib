/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_ALGORITHMS_POINT_SAMPLING_H
#define VCL_ALGORITHMS_POINT_SAMPLING_H

#include <vclib/algorithms/shuffle.h>
#include <vclib/algorithms/stat.h>
#include <vclib/math/random.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/comparators.h>
#include <vclib/space/sampler.h>

/**
 * @defgroup point_sampling Point Sampling Algorithms
 *
 * @ingroup algorithms
 *
 * @brief List of functions that compute point samplings.
 *
 * These functions compute saplings using various simple and complex methods,
 * and return objects that satisfy the @ref SamplerConcept. A Sampler is an
 * object that allows to store points that can be results of some geometric
 * function (e.g. a point sample that is computed as the barycentric coordinate
 * of a triangle).
 *
 * You can access these algorithms by including `#include
 * <vclib/algorithms/point_sampling.h>`
 */

namespace vcl {

/**
 * @brief Returns a Sampler object that contains all the vertices contained in
 * the given mesh.
 *
 * This function creates a SamplerType object that contains all the vertices in
 * the given MeshType object m. If onlySelected is true, only the selected
 * vertices are sampled. The indices of the sampled vertices in the mesh are
 * stored in the birthVertices vector.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the MeshConcept
 *
 * @param[in] m: A const reference to the Mesh object
 * @param[out] birthVertices: A vector of uints that will contain the indices of
 * the sampled vertices in the mesh
 * @param[in] onlySelected: A bool that specifies whether to sample only
 * selected vertices
 *
 * @return A SamplerType object that contains the sampled vertices
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesPointSampling(
    const MeshType&    m,
    std::vector<uint>& birthVertices,
    bool               onlySelected = false)
{
    using VertexType = MeshType::VertexType;

    SamplerType sampler;

    // Determine the number of vertices to sample
    uint n = onlySelected ? vertexSelectionNumber(m) : m.vertexNumber();

    // Reserve space in the sampler and birthVertices vectors
    sampler.reserve(n);
    birthVertices.reserve(n);
    birthVertices.clear();

    // Loop through all the vertices in the mesh and add them to the sampler if
    // they are selected
    for (const VertexType& v : m.vertices()) {
        if (!onlySelected || v.selected()) {
            sampler.add(v);
            birthVertices.push_back(m.index(v));
        }
    }
    return sampler;
}

/**
 * @brief Returns a Sampler object that contains all the vertices contained in
 * the given mesh.
 *
 * This function creates a SamplerType object that contains all the vertices in
 * the given MeshType object m. If onlySelected is true, only the selected
 * vertices are sampled.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the MeshConcept
 *
 * @param[in] m: A const reference to the Mesh object
 * @param[in] onlySelected: A bool that specifies whether to sample only
 * selected vertices
 *
 * @return A SamplerType object that contains the sampled vertices
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesPointSampling(
    const MeshType& m,
    bool            onlySelected = false)
{
    std::vector<uint> v;
    return allVerticesPointSampling<SamplerType>(m, v, onlySelected);
}

/**
 * @brief Returns a SamplerType object that contains all the faces contained in
 * the given mesh.
 *
 * This function creates a SamplerType object that contains sampled points on
 * the faces of the given MeshType object m. If onlySelected is true, only the
 * selected faces are sampled. The indices of the sampled faces in the mesh are
 * stored in the birthFaces vector.
 *
 * The specific sampling method is determined by the implementation of the
 * SamplerType class. However, in general, the sampled point on each face is the
 * face barycenter.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the FaceMeshConcept
 *
 * @param[in] m: A const reference to the Mesh object
 * @param[out] birthFaces: A vector of uints that will contain the indices of
 * the sampled faces in the mesh
 * @param[in] onlySelected: A bool that specifies whether to sample only
 * selected faces
 *
 * @return A SamplerType object that contains the sampled points on the faces
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType allFacesPointSampling(
    const MeshType&    m,
    std::vector<uint>& birthFaces,
    bool               onlySelected = false)
{
    using FaceType = MeshType::FaceType;

    SamplerType sampler;

    // Determine the number of vertices to sample
    uint n = onlySelected ? faceSelectionNumber(m) : m.faceNumber();

    // Reserve space in the sampler and birthVertices vectors
    sampler.reserve(n);
    birthFaces.reserve(n);
    birthFaces.clear();

    for (const FaceType& f : m.faces()) {
        if (!onlySelected || f.selected()) {
            sampler.add(f);
            birthFaces.push_back(m.index(f));
        }
    }
    return sampler;
}

/**
 * @brief Returns a SamplerType object that contains all the faces contained in
 * the given mesh.
 *
 * This function creates a SamplerType object that contains sampled points on
 * the faces of the given MeshType object m. If onlySelected is true, only the
 * selected faces are sampled.
 *
 * The specific sampling method is determined by the implementation of the
 * SamplerType class. However, in general, the sampled point on each face is the
 * face barycenter.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the FaceMeshConcept
 *
 * @param[in] m: A const reference to the Mesh object
 * @param[in] onlySelected: A bool that specifies whether to sample only
 * selected faces
 *
 * @return A SamplerType object that contains the sampled points on the faces
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType allFacesPointSampling(const MeshType& m, bool onlySelected = false)
{
    std::vector<uint> v;
    return allFacesPointSampling<SamplerType>(m, v, onlySelected);
}

/**
 * @brief Returns a SamplerType object that contains the given number of samples
 * taken from the vertices of the given mesh. Each vertex has the same
 * probability of being chosen. If onlySelected is true, only the selected
 * vertices are sampled. The indices of the sampled vertices in the mesh are
 * stored in the birthVertices vector.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the MeshConcept
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] nSamples The number of samples to take.
 * @param[out] birthVertices: A vector of indices of the birth vertices.
 * @param[in] onlySelected: Whether to only sample from the selected vertices.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 * @return A SamplerType object containing the sampled vertices.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexUniformPointSampling(
    const MeshType&    m,
    uint               nSamples,
    std::vector<uint>& birthVertices,
    bool               onlySelected  = false,
    bool               deterministic = false)
{
    uint vn = onlySelected ? vcl::vertexSelectionNumber(m) : m.vertexNumber();

    if (nSamples >= vn) {
        return allVerticesPointSampling<SamplerType>(
            m, birthVertices, onlySelected);
    }

    SamplerType ps;

    // Reserve space in the sampler and birthVertices vectors
    ps.reserve(vn);
    birthVertices.reserve(vn);
    birthVertices.clear();

    std::uniform_int_distribution<uint> dist(0, m.vertexContainerSize() - 1);

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    std::vector<bool> visited(m.vertexContainerSize(), false);
    uint              nVisited = 0;

    while (nVisited < nSamples) {
        uint vi = dist(gen);
        if (!m.vertex(vi).deleted() && !visited[vi] &&
            (!onlySelected || m.vertex(vi).selected()))
        {
            visited[vi] = true;
            nVisited++;
            ps.add(m.vertex(vi));
            birthVertices.push_back(vi);
        }
    }

    return ps;
}

/**
 * @brief Returns a SamplerType object that contains the given number of samples
 * taken from the vertices of the given mesh. Each vertex has the same
 * probability of being chosen. If onlySelected is true, only the selected
 * vertices are sampled.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the MeshConcept
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] nSamples The number of samples to take.
 * @param[in] onlySelected: Whether to only sample from the selected vertices.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 * @return A Sampler object containing the sampled vertices.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexUniformPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            onlySelected  = false,
    bool            deterministic = false)
{
    std::vector<uint> v;
    return vertexUniformPointSampling<SamplerType>(
        m, nSamples, v, onlySelected, deterministic);
}

/**
 * @brief Returns a SamplerType object that contains the given number of samples
 * taken from the faces of the given mesh. Each face has the same probability of
 * being chosen. If onlySelected is true, only the selected faces are sampled.
 * The indices of the sampled faces in the mesh are stored in the birthFaces
 * vector.
 *
 * The specific sampling method is determined by the implementation of the
 * SamplerType class. However, in general, the sampled point on each face is the
 * face barycenter.
 *
 * @tparam SamplerType A type that satisfies the SamplerConcept
 * @tparam MeshType A type that satisfies the FaceMeshConcept
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] nSamples: The number of samples to take.
 * @param[out] birthFaces: A vector of indices of the birth faces.
 * @param[in] onlySelected: Whether to only sample from the selected faces.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 *
 * @return A SamplerType object that contains the sampled points on the faces.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceUniformPointSampling(
    const MeshType&    m,
    uint               nSamples,
    std::vector<uint>& birthFaces,
    bool               onlySelected  = false,
    bool               deterministic = false)
{
    uint fn = onlySelected ? vcl::faceSelectionNumber(m) : m.faceNumber();

    if (nSamples >= fn) {
        return allFacesPointSampling<SamplerType>(m, birthFaces, onlySelected);
    }

    SamplerType ps;

    // Reserve space in the sampler and birthFaces vectors
    ps.reserve(fn);
    birthFaces.reserve(fn);
    birthFaces.clear();

    std::uniform_int_distribution<uint> dist(0, m.faceContainerSize() - 1);

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    std::vector<bool> visited(m.faceContainerSize(), false);
    uint              nVisited = 0;

    while (nVisited < nSamples) {
        uint fi = dist(gen);
        if (!m.face(fi).deleted() && !visited[fi] &&
            (!onlySelected || m.face(fi).selected()))
        {
            visited[fi] = true;
            nVisited++;
            ps.add(m.face(fi));
            birthFaces.push_back(fi);
        }
    }

    return ps;
}

/**
 * @brief Returns a SamplerType object that contains the given number of samples
 * taken from the faces of the given mesh. Each face has the same probability of
 * being chosen. If onlySelected is true, only the selected faces are sampled.
 *
 * The specific sampling method is determined by the implementation of the
 * SamplerType class. However, in general, the sampled point on each face is the
 * face barycenter.
 *
 * @tparam SamplerType A type that satisfies the SamplerConcept
 * @tparam MeshType A type that satisfies the FaceMeshConcept
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] nSamples: The number of samples to take.
 * @param[in] onlySelected: Whether to only sample from the selected faces.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 *
 * @return A SamplerType object that contains the sampled points on the faces.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceUniformPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            onlySelected  = false,
    bool            deterministic = false)
{
    std::vector<uint> v;
    return faceUniformPointSampling<SamplerType>(
        m, nSamples, v, onlySelected, deterministic);
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex weights
 * given as input. Each vertex has a probability of being chosen that is
 * proportional to its weight. If onlySelected is true, only the selected
 * vertices are sampled. The indices of the sampled vertices in the mesh are
 * stored in the birthVertices vector.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the MeshConcept
 * @tparam ScalarType: The scalar type used for the weights
 *
 * @param[in] m: The input mesh to sample from.
 * @param[in] weights: A vector of scalars having the i-th entry associated to
 * the vertex having index i. Note: weights.size() == m.vertexContainerSize().
 * @param[in] nSamples: The number of vertices to sample.
 * @param[out] birthVertices: A vector to store the indices of the vertices that
 * were sampled.
 * @param[in] deterministic: If true, sets the random number generator to a
 * deterministic mode.
 *
 * @return A SamplerType object containing the samples selected from the input
 * mesh.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedPointSampling(
    const MeshType&                m,
    const std::vector<ScalarType>& weights,
    uint                           nSamples,
    std::vector<uint>&             birthVertices,
    bool                           deterministic = false)
{
    if (nSamples >= m.vertexNumber()) {
        return allVerticesPointSampling<SamplerType>(m, birthVertices);
    }

    SamplerType ps;
    ps.reserve(nSamples);
    birthVertices.reserve(nSamples);
    birthVertices.clear();

    std::discrete_distribution<> dist(std::begin(weights), std::end(weights));

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    std::vector<bool> visited(m.vertexContainerSize(), false);
    uint              nVisited = 0;

    while (nVisited < nSamples) {
        uint vi = dist(gen);
        if (vi < m.vertexContainerSize() && !m.vertex(vi).deleted() &&
            !visited[vi])
        {
            visited[vi] = true;
            nVisited++;
            ps.add(m.vertex(vi));
            birthVertices.push_back(vi);
        }
    }

    return ps;
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex weights
 * given as input. Each vertex has a probability of being chosen that is
 * proportional to its weight. If onlySelected is true, only the selected
 * vertices are sampled.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the MeshConcept
 * @tparam ScalarType: The scalar type used for the weights
 *
 * @param[in] m: The input mesh to sample from.
 * @param[in] weights: A vector of scalars having the i-th entry associated to
 * the vertex having index i. Note: weights.size() == m.vertexContainerSize().
 * @param[in] nSamples: The number of vertices to sample.
 * @param[in] deterministic: If true, sets the random number generator to a
 * deterministic mode.
 *
 * @return A SamplerType object containing the samples selected from the input
 * mesh.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType, typename ScalarType>
SamplerType vertexWeightedPointSampling(
    const MeshType&                m,
    const std::vector<ScalarType>& weights,
    uint                           nSamples,
    bool                           deterministic = false)
{
    std::vector<uint> v;
    return vertexWeightedPointSampling<SamplerType>(
        m, weights, nSamples, v, deterministic);
}

/**
 * @brief Returns a SamplerType object that contains the given number of samples
 * taken from the faces of the given mesh. Each face has the same probability of
 * being chosen. If onlySelected is true, only the selected faces are sampled.
 * The indices of the sampled faces in the mesh are stored in the birthFaces
 * vector.
 *
 * The specific sampling method is determined by the implementation of the
 * SamplerType class. However, in general, the sampled point on each face is the
 * face barycenter.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the FaceMeshConcept
 * @tparam ScalarType: The scalar type used for the weights
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] weights: A vector of scalars having the i-th entry associated to
 * the face having index i. Note: weights.size() == m.faceContainerSize().
 * @param[in] nSamples: The number of samples to take.
 * @param[out] birthFaces: A vector to store the indices of the faces that were
 * sampled.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 *
 * @return A SamplerType object that contains the sampled points on the faces.
 *
 * @ingroup point_sampling
 */
template<
    SamplerConcept  SamplerType,
    FaceMeshConcept MeshType,
    typename ScalarType>
SamplerType faceWeightedPointSampling(
    const MeshType&                m,
    const std::vector<ScalarType>& weights,
    uint                           nSamples,
    std::vector<uint>&             birthFaces,
    bool                           deterministic = false)
{
    if (nSamples >= m.faceNumber()) {
        return allFacesPointSampling<SamplerType>(m);
    }

    SamplerType ps;

    // Reserve space in the sampler and birthFaces vectors
    ps.reserve(nSamples);
    birthFaces.reserve(nSamples);
    birthFaces.clear();

    std::discrete_distribution<> dist(std::begin(weights), std::end(weights));

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    std::vector<bool> visited(m.faceContainerSize(), false);
    uint              nVisited = 0;

    while (nVisited < nSamples) {
        uint fi = dist(gen);
        if (fi < m.faceContainerSize() && !m.face(fi).deleted() && !visited[fi])
        {
            visited[fi] = true;
            nVisited++;
            ps.add(m.face(fi));
            birthFaces.push_back(fi);
        }
    }

    return ps;
}

/**
 * @brief Returns a SamplerType object that contains the given number of samples
 * taken from the faces of the given mesh. Each face has the same probability of
 * being chosen. If onlySelected is true, only the selected faces are sampled.
 *
 * The specific sampling method is determined by the implementation of the
 * SamplerType class. However, in general, the sampled point on each face is the
 * face barycenter.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the FaceMeshConcept
 * @tparam ScalarType: The scalar type used for the weights
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] weights: A vector of scalars having the i-th entry associated to
 * the face having index i. Note: weights.size() == m.faceContainerSize().
 * @param[in] nSamples: The number of samples to take.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 *
 * @return A SamplerType object that contains the sampled points on the faces.
 *
 * @ingroup point_sampling
 */
template<
    SamplerConcept  SamplerType,
    FaceMeshConcept MeshType,
    typename ScalarType>
SamplerType faceWeightedPointSampling(
    const MeshType&                m,
    const std::vector<ScalarType>& weights,
    uint                           nSamples,
    bool                           deterministic = false)
{
    std::vector<uint> v;
    return faceWeightedPointSampling<SamplerType>(
        m, weights, nSamples, v, deterministic);
}

/**
 * @brief Samples the vertices in a weighted way, using the per vertex Quality
 * component. Each vertex has a probability of being chosen that is proportional
 * to its quality value.
 *
 * @param m
 * @param nSamples
 * @return
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexQualityWeightedPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    vcl::requirePerVertexQuality(m);

    using VertexType  = MeshType::VertexType;
    using QualityType = VertexType::QualityType;

    std::vector<QualityType> weights;
    weights.resize(m.vertexContainerSize(), 0);
    for (const VertexType& v : m.vertices()) {
        weights[m.index(v)] = v.quality();
    }

    return vertexWeightedPointSampling<SamplerType>(
        m, weights, nSamples, deterministic);
}

/**
 * @brief Samples the faces in a weighted way, using the per face Quality
 * component. Each face has a probability of being chosen that is proportional
 * to its quality value.
 *
 * @param m
 * @param nSamples
 * @return
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceQualityWeightedPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    vcl::requirePerFaceQuality(m);

    using FaceType    = MeshType::FaceType;
    using QualityType = FaceType::QualityType;

    std::vector<QualityType> weights;
    weights.resize(m.faceContainerSize(), 0);
    for (const FaceType& f : m.faces()) {
        weights[m.index(f)] = f.quality();
    }

    return faceWeightedPointSampling<SamplerType>(
        m, weights, nSamples, deterministic);
}

/**
 * @brief Samples the vertices in a weighted way, using the area. Each vertex
 * has a probability of being chosen that is proportional to the average area of
 * its adjacent faces.
 *
 * @param m
 * @param nSamples
 * @return
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType vertexAreaWeightedPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    using VertexType = MeshType::VertexType;
    using ScalarType = VertexType::ScalarType;
    using FaceType   = MeshType::FaceType;

    std::vector<ScalarType> weights(m.vertexContainerSize(), 0);
    std::vector<uint>       cnt(m.vertexContainerSize(), 0);

    // for each vertex, store in weights the adjacent faces area and their
    // number
    for (const FaceType& f : m.faces()) {
        ScalarType area = vcl::faceArea(f);
        for (const VertexType* v : f.vertices()) {
            weights[m.index(v)] += area;
            cnt[m.index(v)]++;
        }
    }

    // divide each area sum by the number of adjacent faces
    for (uint i = 0; i < weights.size(); i++) {
        if (cnt[i] > 0)
            weights[i] /= cnt[i];
    }

    // use these weights to create a sapler
    return vertexWeightedPointSampling<SamplerType>(
        m, weights, nSamples, deterministic);
}

/**
 * @brief Samples the faces in a weighted way, using the per face area. Each
 * face has a probability of being chosen that is proportional to its area.
 *
 * @param m
 * @param nSamples
 * @return
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType faceAreaWeightedPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    using FaceType = MeshType::FaceType;

    std::vector<double> weights(m.faceContainerSize());

    for (const FaceType& f : m.faces()) {
        weights[m.index(f)] = vcl::faceArea(f);
    }

    return faceWeightedPointSampling<SamplerType>(
        m, weights, nSamples, deterministic);
}

/**
 * @brief Computes a montecarlo distribution with an exact number of samples. It
 * works by generating a sequence of consecutive segments proportional to the
 * face areas and actually shooting sample over this line. The indices of the
 * sampled faces in the mesh are stored in the birthFaces vector.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the FaceMeshConcept
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] nSamples: The number of samples to take.
 * @param[out] birthFaces: A vector to store the indices of the faces that were
 * sampled.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 *
 * @return A SamplerType object that contains the sampled points on the faces.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType montecarloPointSampling(
    const MeshType&    m,
    uint               nSamples,
    std::vector<uint>& birthFaces,
    bool               deterministic = false)
{
    using VertexType = MeshType::VertexType;
    using ScalarType = VertexType::CoordType::ScalarType;
    using FaceType   = MeshType::FaceType;
    using Interval   = std::pair<ScalarType, const FaceType*>;

    vcl::FirstElementPairComparator<Interval> comparator;
    SamplerType                               sampler;

    // Reserve space in the sampler and birthFaces vectors
    sampler.reserve(nSamples);
    birthFaces.reserve(nSamples);
    birthFaces.clear();

    std::uniform_real_distribution<ScalarType> dist(0, 1);

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    std::vector<Interval> intervals(m.faceNumber());
    uint                  i    = 0;
    ScalarType            area = 0;
    for (const FaceType& f : m.faces()) {
        area += vcl::faceArea(f);
        intervals[i] = std::make_pair(area, &f);
        i++;
    }

    ScalarType meshArea = intervals.back().first;
    for (uint i = 0; i < nSamples; i++) {
        ScalarType val = meshArea * dist(gen);
        // lower_bound returns the furthermost iterator i in [first, last) such
        // that, for every iterator j in [first, i), *j < value. E.g. An
        // iterator pointing to the first element "not less than" val, or end()
        // if every element is less than val.
        typename std::vector<Interval>::iterator it = std::lower_bound(
            intervals.begin(),
            intervals.end(),
            std::make_pair(val, nullptr),
            comparator);

        sampler.add(
            *it->second,
            vcl::randomPolygonBarycentricCoordinate<ScalarType>(
                it->second->vertexNumber(), gen));
        birthFaces.push_back(it->second->index());
    }

    return sampler;
}

/**
 * @brief Computes a montecarlo distribution with an exact number of samples. It
 * works by generating a sequence of consecutive segments proportional to the
 * face areas and actually shooting sample over this line.
 *
 * @tparam SamplerType: A type that satisfies the SamplerConcept
 * @tparam MeshType: A type that satisfies the FaceMeshConcept
 *
 * @param[in] m: The mesh to sample from.
 * @param[in] nSamples: The number of samples to take.
 * @param[in] deterministic: Whether to use a deterministic random generator.
 *
 * @return A SamplerType object that contains the sampled points on the faces.
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType montecarloPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    std::vector<uint> birthFaces;
    return montecarloPointSampling<SamplerType>(
        m, nSamples, birthFaces, deterministic);
}

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType stratifiedMontecarloPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    using FaceType   = MeshType::FaceType;
    using ScalarType = SamplerType::ScalarType;

    SamplerType ps;

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    double area              = vcl::surfaceArea(m);
    double samplePerAreaUnit = nSamples / area;
    // Montecarlo sampling.
    double floatSampleNum = 0.0;

    for (const FaceType& f : m.faces()) {
        // compute # samples in the current face (taking into account of the
        // remainders)
        floatSampleNum += vcl::faceArea(f) * samplePerAreaUnit;
        int faceSampleNum = (int) floatSampleNum;
        // for every sample p_i in T...
        for (int i = 0; i < faceSampleNum; i++)
            ps.add(
                f,
                vcl::randomPolygonBarycentricCoordinate<ScalarType>(
                    f.vertexNumber(), gen));
        floatSampleNum -= (double) faceSampleNum;
    }

    return ps;
}

/**
 * @brief This function compute montecarlo distribution with an approximate
 * number of samples exploiting the poisson distribution approximation of the
 * binomial distribution.
 *
 * For a given triangle t of area a_t, in a Mesh of area A,
 * if we take n_s sample over the mesh, the number of samples that falls in t
 * follows the poisson distribution of P(lambda ) with lambda = n_s * (a_t/A).
 *
 * To approximate the Binomial we use a Poisson distribution with parameter
 * \lambda = np can be used as an approximation to B(n,p)
 * (it works if n is sufficiently large and p is sufficiently small).
 *
 *
 * @param m
 * @param nSamples
 * @param deterministic
 * @return
 *
 * @ingroup point_sampling
 */
template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType montecarloPoissonPointSampling(
    const MeshType& m,
    uint            nSamples,
    bool            deterministic = false)
{
    using FaceType   = MeshType::FaceType;
    using ScalarType = SamplerType::ScalarType;

    SamplerType ps;

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    ScalarType area              = vcl::surfaceArea(m);
    ScalarType samplePerAreaUnit = nSamples / area;

    for (const FaceType& f : m.faces()) {
        ScalarType areaT = vcl::faceArea(f);
        int        faceSampleNum =
            vcl::poissonRandomNumber(areaT * samplePerAreaUnit, gen);

        // for every sample p_i in T...
        for (int i = 0; i < faceSampleNum; i++)
            ps.add(
                f,
                vcl::randomPolygonBarycentricCoordinate<ScalarType>(
                    f.vertexNumber(), gen));
    }

    return ps;
}

template<
    SamplerConcept  SamplerType,
    FaceMeshConcept MeshType,
    typename ScalarType>
SamplerType vertexWeightedMontecarloPointSampling(
    const MeshType&                m,
    const std::vector<ScalarType>& weights,
    uint                           nSamples,
    double                         variance,
    bool                           deterministic = false)
{
    using FaceType = MeshType::FaceType;

    // lambda function to compute radius weighted area of a face
    auto weightedArea = [](const FaceType&                f,
                           const MeshType&                m,
                           const std::vector<ScalarType>& r) -> ScalarType {
        ScalarType averageQ = 0;
        for (uint i = 0; i < f.vertexNumber(); ++i)
            averageQ += r[m.index(f.vertex(i))];

        averageQ /= f.vertexNumber();
        return averageQ * averageQ * vcl::faceArea(f);
    };

    SamplerType ps;

    std::random_device rd;
    std::mt19937       gen(rd());
    if (deterministic)
        gen = std::mt19937(0);

    std::vector<ScalarType> radius =
        vcl::vertexRadiusFromWeights(m, weights, 1.0, variance, true);

    ScalarType wArea = 0;
    for (const FaceType& f : m.faces())
        wArea += weightedArea(f, m, radius);

    ScalarType samplePerAreaUnit = nSamples / wArea;
    // Montecarlo sampling.
    double floatSampleNum = 0.0;
    for (const FaceType& f : m.faces()) {
        // compute # samples in the current face (taking into account of the
        // remainders)
        floatSampleNum += weightedArea(f, m, radius) * samplePerAreaUnit;
        uint faceSampleNum = (uint) floatSampleNum;

        // for every sample p_i in T...
        for (uint i = 0; i < faceSampleNum; i++)
            ps.add(
                f,
                vcl::randomPolygonBarycentricCoordinate<ScalarType>(
                    f.vertexNumber(), gen));

        floatSampleNum -= (double) faceSampleNum;
    }

    return ps;
}

template<SamplerConcept SamplerType, FaceMeshConcept MeshType>
SamplerType vertexQualityWeightedMontecarloPointSampling(
    const MeshType& m,
    uint            nSamples,
    double          variance,
    bool            deterministic = false)
{
    vcl::requirePerVertexQuality(m);

    using VertexType  = MeshType::VertexType;
    using QualityType = VertexType::QualityType;

    std::vector<QualityType> weights;
    weights.resize(m.vertexContainerSize(), 0);
    for (const VertexType& v : m.vertices()) {
        weights[m.index(v)] = v.quality();
    }

    return vertexWeightedMontecarloPointSampling<SamplerType>(
        m, weights, nSamples, variance, deterministic);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_POINT_SAMPLING_H
