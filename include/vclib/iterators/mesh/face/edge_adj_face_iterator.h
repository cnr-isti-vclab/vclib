/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H
#define VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

template<typename FaceType, bool CNST = false>
class EdgeAdjFaceIterator
{
    using FT = std::conditional_t<CNST, FaceType, const FaceType>;

    using VT = std::conditional_t<CNST,
        typename FT::VertexType,
        const typename FT::VertexType>;

    FT* current = nullptr;
    FT* end     = nullptr;
    VT* v0      = nullptr;
    VT* v1      = nullptr;

public:
    using difference_type   = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using value_type        = FT*;
    using reference         = FT*&;
    using pointer           = FT**;

    EdgeAdjFaceIterator() = default;

    EdgeAdjFaceIterator(FT& f, uint edge) :
            current(&f), end(&f), v0(f.vertex(edge)), v1(f.vertexMod(edge + 1))
    {
    }

    bool operator==(const EdgeAdjFaceIterator& oi) const
    {
        return current == oi.current && v0 == oi.v0 && v1 == oi.v1;
    }

    bool operator!=(const EdgeAdjFaceIterator& oi) const
    {
        return !(*this == oi);
    }

    EdgeAdjFaceIterator& operator++()
    {
        assert(current);
        uint edge = current->indexOfEdge(v0, v1);
        assert(edge != UINT_NULL);
        current = current->adjFace(edge);
        if (current == end || current == nullptr) {
            current = nullptr;
            v0 = nullptr;
            v1 = nullptr;
        }
        return *this;
    }

    EdgeAdjFaceIterator operator++(int)
    {
        auto it = *this;
        ++(*this);
        return it;
    }

    reference operator*() const { return current; }

    pointer operator->() const { return &current; }
};

template<typename FaceType>
using ConstEdgeAdjFaceIterator = EdgeAdjFaceIterator<FaceType, true>;

} // namespace vcl

#endif // VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H
