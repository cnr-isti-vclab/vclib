// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ITERATORS_FACE_EDGE_ADJ_FACE_ITERATOR_H
#define VCL_MESH_ITERATORS_FACE_EDGE_ADJ_FACE_ITERATOR_H

#include <vclib/base.h>

#include <iterator>

namespace vcl {

template<typename FaceType, bool CNST = false>
class EdgeAdjFaceIterator
{
    using FT = std::conditional_t<CNST, const FaceType, FaceType>;

    using VT = std::conditional_t<
        CNST,
        const typename FT::VertexType,
        typename FT::VertexType>;

    FT* mCurrent = nullptr;
    FT* mEnd     = nullptr;
    VT* mV0      = nullptr;
    VT* mV1      = nullptr;

public:
    using difference_type   = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using value_type        = FT*;
    using reference         = FT*&;
    using pointer           = FT**;

    EdgeAdjFaceIterator() = default;

    EdgeAdjFaceIterator(FT& f, uint edge) :
            mCurrent(&f), mEnd(&f), mV0(f.vertex(edge)),
            mV1(f.vertexMod(edge + 1))
    {
    }

    bool operator==(const EdgeAdjFaceIterator& oi) const
    {
        return mCurrent == oi.mCurrent && mV0 == oi.mV0 && mV1 == oi.mV1;
    }

    bool operator!=(const EdgeAdjFaceIterator& oi) const
    {
        return !(*this == oi);
    }

    EdgeAdjFaceIterator& operator++()
    {
        assert(mCurrent);
        uint edge = mCurrent->indexOfEdge(mV0, mV1);
        assert(edge != UINT_NULL);
        mCurrent = mCurrent->adjFace(edge);
        if (mCurrent == mEnd || mCurrent == nullptr) {
            mCurrent = nullptr;
            mV0      = nullptr;
            mV1      = nullptr;
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

    pointer operator->() const { return &mCurrent; }
};

template<typename FaceType>
using ConstEdgeAdjFaceIterator = EdgeAdjFaceIterator<FaceType, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_FACE_EDGE_ADJ_FACE_ITERATOR_H
