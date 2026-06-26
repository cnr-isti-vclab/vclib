// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_POINT_SAMPLER_H
#define VCL_SPACE_COMPLEX_POINT_SAMPLER_H

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief The PointSampler class provides a simple container for collecting
 * point samples, with a set of convenience methods to add or overwrite samples
 * from raw points, mesh vertices, edges, and faces.
 *
 * Samples can be appended via the various `add()` overloads or written to a
 * specific position via the corresponding `set()` overloads. The class exposes
 * range-based iteration and can be converted to any mesh type that satisfies
 * `MeshConcept`.
 *
 * The class satisfies the `PointSamplerConcept`.
 *
 * @tparam PointT: The point type used to store each sample. Must satisfy
 * `PointConcept`. Defaults to `Point3d`.
 *
 * @ingroup space_complex
 */
template<PointConcept PointT = Point3d>
class PointSampler
{
    std::vector<PointT> mSamples;

public:
    /// The point type used to store each sample.
    using PointType = PointT;

    /// The scalar type of the point coordinates.
    using ScalarType = PointT::ScalarType;

    /// Constant iterator over the sample container.
    using ConstIterator = std::vector<PointT>::const_iterator;

    /**
     * @brief Default constructor. Creates an empty PointSampler with no
     * samples.
     */
    PointSampler() = default;

    /**
     * @brief Returns a const reference to the internal vector of samples.
     * @return A const reference to the `std::vector<PointT>` holding all
     * samples.
     */
    const std::vector<PointT>& samples() const { return mSamples; }

    /**
     * @brief Returns a const reference to the i-th sample.
     * @param[in] i: Index of the sample to retrieve.
     * @return A const reference to the i-th sample point.
     */
    const PointT& sample(uint i) const { return mSamples[i]; }

    /**
     * @brief Returns the number of samples currently stored.
     * @return The number of samples.
     */
    std::size_t size() const { return mSamples.size(); }

    /**
     * @brief Removes all samples from the container.
     */
    void clear() { mSamples.clear(); }

    /**
     * @brief Reserves capacity for at least `n` samples, avoiding
     * reallocations when the final number of samples is known in advance.
     * @param[in] n: The minimum number of samples to reserve space for.
     */
    void reserve(uint n) { mSamples.reserve(n); }

    /**
     * @brief Resizes the sample container to hold exactly `n` samples.
     * New samples are default-constructed; excess samples are discarded.
     * @param[in] n: The new size of the container.
     */
    void resize(uint n) { mSamples.resize(n); }

    /**
     * @brief Appends a copy of the given point to the sample list.
     * @param[in] p: The point to add.
     */
    void add(const PointT& p) { mSamples.push_back(p); }

    /**
     * @brief Overwrites the i-th sample with a copy of the given point.
     * @param[in] i: Index of the sample to overwrite.
     * @param[in] p: The point to store at position `i`.
     */
    void set(uint i, const PointT& p) { mSamples[i] = p; }

    /**
     * @brief Appends a sample at the position of the given vertex.
     * @tparam VertexType: A type satisfying `VertexConcept`.
     * @param[in] v: The vertex whose position is added as a sample.
     */
    template<VertexConcept VertexType>
    void add(const VertexType& v)
    {
        mSamples.push_back(v.position());
    }

    /**
     * @brief Overwrites the i-th sample with the position of the given vertex.
     * @tparam VertexType: A type satisfying `VertexConcept`.
     * @param[in] i: Index of the sample to overwrite.
     * @param[in] v: The vertex whose position is stored at position `i`.
     */
    template<VertexConcept VertexType>
    void set(uint i, const VertexType& v)
    {
        mSamples[i] = v.position();
    }

    /**
     * @brief Appends a sample interpolated along the given edge.
     *
     * The sample is computed as `v0 * (1 - u) + v1 * u`, where `v0` and `v1`
     * are the positions of the two endpoints of the edge.
     *
     * @tparam EdgeType: A type satisfying `EdgeConcept`.
     * @param[in] e: The edge to sample.
     * @param[in] u: Parametric coordinate in [0, 1] along the edge. Defaults
     * to `0.5` (midpoint).
     */
    template<EdgeConcept EdgeType>
    void add(const EdgeType& e, double u = 0.5)
    {
        mSamples.push_back(
            (e.vertex(0).position() * (1 - u)) + (e.vertex(1).position() * u));
    }

    /**
     * @brief Overwrites the i-th sample with an interpolated point along the
     * given edge.
     *
     * The sample is computed as `v0 * (1 - u) + v1 * u`, where `v0` and `v1`
     * are the positions of the two endpoints of the edge.
     *
     * @tparam EdgeType: A type satisfying `EdgeConcept`.
     * @param[in] i: Index of the sample to overwrite.
     * @param[in] e: The edge to sample.
     * @param[in] u: Parametric coordinate in [0, 1] along the edge. Defaults
     * to `0.5` (midpoint).
     */
    template<EdgeConcept EdgeType>
    void set(uint i, const EdgeType& e, double u = 0.5)
    {
        mSamples[i] =
            (e.vertex(0).position() * (1 - u)) + (e.vertex(1).position() * u);
    }

    /**
     * @brief Appends a sample at the barycenter of the given face.
     * @tparam FaceType: A type satisfying `FaceConcept`.
     * @param[in] f: The face whose barycenter is added as a sample.
     */
    template<FaceConcept FaceType>
    void add(const FaceType& f)
    {
        mSamples.push_back(faceBarycenter(f));
    }

    /**
     * @brief Overwrites the i-th sample with the barycenter of the given face.
     * @tparam FaceType: A type satisfying `FaceConcept`.
     * @param[in] i: Index of the sample to overwrite.
     * @param[in] f: The face whose barycenter is stored at position `i`.
     */
    template<FaceConcept FaceType>
    void set(uint i, const FaceType& f)
    {
        mSamples[i] = faceBarycenter(f);
    }

    /**
     * @brief Appends a sample on the given face using per-vertex barycentric
     * coordinates supplied as a vector.
     *
     * The sample is computed as `sum_k( v_k * barCoords[k] )` over all
     * vertices of the face. The method asserts that `barCoords` contains at
     * least as many entries as the vertex count of the face.
     *
     * @tparam FaceType: A type satisfying `FaceConcept`.
     * @param[in] f: The face on which the sample is placed.
     * @param[in] barCoords: A vector of barycentric weights, one per vertex of
     * `f`. Must have size >= `f.vertexCount()`.
     */
    template<FaceConcept FaceType>
    void add(const FaceType& f, const std::vector<ScalarType>& barCoords)
    {
        assert(f.vertexCount() <= barCoords.size());

        PointT p;
        for (uint i = 0; i < f.vertexCount(); i++)
            p += f.vertex(i)->position() * barCoords[i];

        mSamples.push_back(p);
    }

    /**
     * @brief Overwrites the i-th sample on the given face using per-vertex
     * barycentric coordinates supplied as a vector.
     *
     * The sample is computed as `sum_k( v_k * barCoords[k] )` over all
     * vertices of the face. The method asserts that `barCoords` contains at
     * least as many entries as the vertex count of the face.
     *
     * @tparam FaceType: A type satisfying `FaceConcept`.
     * @param[in] i: Index of the sample to overwrite.
     * @param[in] f: The face on which the sample is placed.
     * @param[in] barCoords: A vector of barycentric weights, one per vertex of
     * `f`. Must have size >= `f.vertexCount()`.
     */
    template<FaceConcept FaceType>
    void set(
        uint                           i,
        const FaceType&                f,
        const std::vector<ScalarType>& barCoords)
    {
        assert(f.vertexCount() <= barCoords.size());

        PointT p;
        for (uint i = 0; i < f.vertexCount(); i++)
            p += f.vertex(i)->position() * barCoords[i];

        mSamples[i] = p;
    }

    /**
     * @brief Appends a sample on the given triangular face using barycentric
     * coordinates stored in a point.
     *
     * The face must be a triangle (either statically typed with `NV == 3` or a
     * polygon asserted to have exactly 3 vertices at runtime). The sample
     * position is computed via `triangleBarycentricCoordinatePoint()`.
     *
     * @tparam FaceType: A type satisfying `FaceConcept` with `NV == 3` or
     * `NV == -1` (polygon, asserted to be triangular at runtime).
     * @param[in] f: The triangular face on which the sample is placed.
     * @param[in] barCoords: Barycentric coordinates as a point, whose
     * components are the weights for the three vertices of `f`.
     */
    template<FaceConcept FaceType>
    void add(const FaceType& f, const PointT& barCoords)
    {
        static_assert(FaceType::NV == 3 || FaceType::NV == -1);
        if constexpr (FaceType::NV == -1) {
            assert(f.vertexCount() == 3);
        }

        PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

        mSamples.push_back(p);
    }

    /**
     * @brief Overwrites the i-th sample on the given triangular face using
     * barycentric coordinates stored in a point.
     *
     * The face must be a triangle (either statically typed with `NV == 3` or a
     * polygon asserted to have exactly 3 vertices at runtime). The sample
     * position is computed via `triangleBarycentricCoordinatePoint()`.
     *
     * @tparam FaceType: A type satisfying `FaceConcept` with `NV == 3` or
     * `NV == -1` (polygon, asserted to be triangular at runtime).
     * @param[in] i: Index of the sample to overwrite.
     * @param[in] f: The triangular face on which the sample is placed.
     * @param[in] barCoords: Barycentric coordinates as a point, whose
     * components are the weights for the three vertices of `f`.
     */
    template<FaceConcept FaceType>
    void set(uint i, const FaceType& f, const PointT& barCoords)
    {
        static_assert(FaceType::NV == 3 || FaceType::NV == -1);
        if constexpr (FaceType::NV == -1) {
            assert(f.vertexCount() == 3);
        }

        PointT p = triangleBarycentricCoordinatePoint(f, barCoords);

        mSamples[i] = p;
    }

    /**
     * @brief Converts the collected samples into a mesh by adding one vertex
     * per sample.
     *
     * The returned mesh has its vertex positions set to the sample points.
     * Vertex capacity is reserved before insertion for efficiency.
     *
     * @tparam MeshType: A type satisfying `MeshConcept`.
     * @return A new `MeshType` mesh containing one vertex for each sample.
     */
    template<MeshConcept MeshType>
    MeshType toMesh() const
    {
        MeshType mesh;
        mesh.reserveVertices(this->size());
        for (const auto& p : mSamples) {
            mesh.addVertex(p);
        }
        return mesh;
    }

    /**
     * @brief Returns a const iterator pointing to the first sample.
     * @return A `ConstIterator` to the beginning of the sample container.
     */
    ConstIterator begin() const { return mSamples.begin(); }

    /**
     * @brief Returns a const iterator pointing past the last sample.
     * @return A `ConstIterator` to the end of the sample container.
     */
    ConstIterator end() const { return mSamples.end(); }
};

/**
 * @brief A concept representing a generic PointSampler.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a PointSampler class having any Point type.
 *
 * @tparam T: The type to be tested for conformity to the PointSamplerConcept.
 *
 * @ingroup space_complex
 */
template<typename T>
concept PointSamplerConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    PointSampler<typename RemoveRef<T>::PointType>>;

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_POINT_SAMPLER_H
