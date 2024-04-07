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

#ifndef VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H
#define VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

template<typename FaceType, bool CNST = false>
class EdgeAdjFaceIterator
{
    using FT = std::conditional_t<CNST, FaceType, const FaceType>;

    using VT = std::conditional_t<
        CNST,
        typename FT::VertexType,
        const typename FT::VertexType>;

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

#endif // VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H
