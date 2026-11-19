// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ITERATORS_FACE_EDGE_ADJ_FACE_ITERATOR_H
#define VCL_MESH_ITERATORS_FACE_EDGE_ADJ_FACE_ITERATOR_H

#include <vclib/base.h>
#include <vclib/mesh/elements/face.h>
#include <vclib/mesh/traits.h>

#include <cassert>
#include <iterator>
#include <type_traits>

namespace vcl {

/**
 * @brief Forward iterator that circulates around the faces adjacent to a given
 * edge of a face.
 *
 * Starting from a face `f` and an edge index `edge`, it iterates over all faces
 * sharing that edge in a non-manifold or 2-manifold mesh.
 *
 * If `FaceType` is const or a ConstFace proxy, the iterator automatically
 * yields const face proxies (`ConstFaceType<FaceType>`). If `FaceType` is
 * mutable, it yields mutable face proxies (`FaceType<FaceType>`).
 *
 * @tparam FaceType: The face element type. Must satisfy FaceConcept and have
 * AdjacentFaces component.
 * @tparam CNST: Explicit override for constness (defaults to constness of FaceType).
 *
 * @ingroup iterators
 */
template<FaceConcept FaceType, bool CNST = std::is_const_v<FaceType>>
    requires comp::HasAdjacentFaces<FaceType>
class EdgeAdjFaceIterator
{
public:
    static constexpr bool IS_CONST =
        CNST ||
        std::is_const_v<FaceType> ||
        std::is_const_v<std::remove_reference_t<
            decltype(std::declval<FaceType>().parentContainer())>>;

    using FT = std::conditional_t<
        IS_CONST,
        vcl::ConstFaceType<FaceType>,
        vcl::FaceType<FaceType>>;

    using VT = std::conditional_t<
        IS_CONST,
        ConstVertexType<FaceType>,
        VertexType<FaceType>>;

    using difference_type   = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using iterator_concept  = std::forward_iterator_tag;
    using value_type        = FT;
    using reference         = FT;
    using pointer           = FakePointerWithValue<FT>;

private:
    FT mCurrent;
    FT mEnd;
    VT mV0;
    VT mV1;

public:
    /**
     * @brief Default constructor creating an invalid (end) iterator.
     */
    EdgeAdjFaceIterator() = default;

    /**
     * @brief Constructs an iterator starting at face `f` on edge `edge`.
     *
     * @param[in] f: The starting face.
     * @param[in] edge: The edge index on `f`.
     */
    EdgeAdjFaceIterator(FT f, uint edge) :
            mCurrent(f), mEnd(f), mV0(f.vertex(edge)),
            mV1(f.vertexMod(edge + 1))
    {
        assert(f);
        assert(edge < f.vertexCount());
    }

    /**
     * @brief Conversion constructor from mutable iterator to const iterator.
     */
    template<FaceConcept OtherFaceType, bool OtherConst>
        requires (IS_CONST && !OtherConst)
    EdgeAdjFaceIterator(
        const EdgeAdjFaceIterator<OtherFaceType, OtherConst>& oi) :
            mCurrent(oi.current()), mEnd(oi.end()), mV0(oi.v0()), mV1(oi.v1())
    {
    }

    /**
     * @brief Equality comparison between two edge-adjacent face iterators of the same type.
     */
    bool operator==(const EdgeAdjFaceIterator& oi) const
    {
        return mCurrent == oi.mCurrent && mV0 == oi.mV0 && mV1 == oi.mV1;
    }

    /**
     * @brief Equality comparison between two edge-adjacent face iterators of different constness.
     */
    template<FaceConcept OtherFaceType, bool OtherConst>
    bool operator==(
        const EdgeAdjFaceIterator<OtherFaceType, OtherConst>& oi) const
    {
        return mCurrent == oi.current() && mV0 == oi.v0() && mV1 == oi.v1();
    }

    /**
     * @brief Advances to the next adjacent face sharing the edge (mV0, mV1).
     */
    EdgeAdjFaceIterator& operator++()
    {
        assert(mCurrent);
        uint edge = mCurrent.indexOfEdge(mV0, mV1);
        assert(edge != UINT_NULL);
        mCurrent = mCurrent.adjFace(edge);
        if (mCurrent == mEnd || !mCurrent) {
            mCurrent = FT();
            mV0      = VT();
            mV1      = VT();
        }
        return *this;
    }

    EdgeAdjFaceIterator operator++(int)
    {
        auto it = *this;
        ++(*this);
        return it;
    }

    reference operator*() const { return mCurrent; }

    pointer operator->() const { return pointer(mCurrent); }

    const FT& current() const { return mCurrent; }
    const FT& end() const { return mEnd; }
    const VT& v0() const { return mV0; }
    const VT& v1() const { return mV1; }
};

// Deduction guides
template<FaceConcept FaceType>
EdgeAdjFaceIterator(const FaceType&, uint)
    -> EdgeAdjFaceIterator<FaceType, true>;

template<FaceConcept FaceType>
EdgeAdjFaceIterator(FaceType&, uint)
    -> EdgeAdjFaceIterator<FaceType, false>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_FACE_EDGE_ADJ_FACE_ITERATOR_H
