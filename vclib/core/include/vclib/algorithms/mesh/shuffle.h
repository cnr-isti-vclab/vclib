// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_SHUFFLE_H
#define VCL_ALGORITHMS_MESH_SHUFFLE_H

#include <vclib/mesh.h>

namespace vcl {

namespace detail {

/*
 * Generic implementation of fillAndShuffleVertexPointerVector, to avoid code
 * duplication.
 *
 * Templates M and V can be both const and non-const MeshType and VertexType
 */
template<typename M, typename V>
std::vector<V*> genericFASVPV(M m, RandomConfig config = std::monostate())
{
    std::vector<V*> vec;
    vec.reserve(m.vertexCount());

    for (V& v : m.vertices()) {
        vec.push_back(&v);
    }

    shuffle(vec, config);

    return vec;
}

/*
 * Generic implementation of fillAndShuffleFacePointerVector, to avoid code
 * duplication.
 *
 * Templates M and F can be both const and non-const MeshType and FaceType
 */
template<typename M, typename F>
std::vector<F*> genericFASFPV(M m, RandomConfig config = std::monostate())
{
    std::vector<F*> vec;
    vec.reserve(m.faceCount());

    for (F& f : m.faces()) {
        vec.push_back(&f);
    }

    shuffle(vec, config);

    return vec;
}

} // namespace detail

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType*> fillAndShuffleVertexPointerVector(
    MeshType&    m,
    RandomConfig config = std::monostate())
{
    using VertexType = MeshType::VertexType;

    return detail::genericFASVPV<MeshType&, VertexType>(m, config);
}

template<MeshConcept MeshType>
std::vector<const typename MeshType::VertexType*>
fillAndShuffleVertexPointerVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    using VertexType = MeshType::VertexType;
    return detail::genericFASVPV<const MeshType&, const VertexType>(m, config);
}

template<MeshConcept MeshType>
std::vector<uint> fillAndShuffleVertexIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    using VertexType = MeshType::VertexType;

    std::vector<uint> vec;
    vec.reserve(m.vertexCount());

    for (const VertexType& v : m.vertices()) {
        vec.push_back(m.index(v));
    }

    shuffle(vec, config);

    return vec;
}

template<FaceMeshConcept MeshType>
std::vector<typename MeshType::FaceType*> fillAndShuffleFacePointerVector(
    MeshType&    m,
    RandomConfig config = std::monostate())
{
    using FaceType = MeshType::FaceType;

    return detail::genericFASFPV<MeshType&, FaceType>(m, config);
}

template<FaceMeshConcept MeshType>
std::vector<const typename MeshType::FaceType*> fillAndShuffleFacePointerVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    using FaceType = MeshType::FaceType;

    return detail::genericFASFPV<const MeshType&, const FaceType>(m, config);
}

template<FaceMeshConcept MeshType>
std::vector<uint> fillAndShuffleFaceIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    using FaceType = MeshType::FaceType;

    std::vector<uint> vec;
    vec.reserve(m.faceCount());

    for (const FaceType& f : m.faces()) {
        vec.push_back(m.index(f));
    }

    shuffle(vec, config);

    return vec;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_SHUFFLE_H
