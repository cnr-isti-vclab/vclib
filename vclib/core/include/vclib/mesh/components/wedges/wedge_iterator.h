// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_WEDGES_WEDGE_ITERATOR_H
#define VCL_MESH_COMPONENTS_WEDGES_WEDGE_ITERATOR_H

#include "wedge_proxy.h"

#include <vclib/base.h>

#include <iterator>
#include <utility>

namespace vcl::comp {

/**
 * @brief Random-access iterator over the wedges of a Face.
 *
 * Dereferences to a lightweight WedgeProxy<FaceType>.
 *
 * @tparam FaceType: The type of the parent Face (mutable or const).
 */
template<typename FaceType>
class WedgeIterator
{
    FaceType mFace;
    uint     mIndex = 0;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = WedgeProxy<FaceType>;
    using difference_type   = std::ptrdiff_t;
    using reference         = WedgeProxy<FaceType>;
    using pointer           = void;

    template<typename>
    friend class WedgeIterator;

    WedgeIterator() = default;

    WedgeIterator(FaceType face, uint index) : mFace(face), mIndex(index) {}

    // Converting constructor from mutable to const (or convertible) FaceType
    template<typename OtherFace>
    requires std::convertible_to<OtherFace, FaceType>
    WedgeIterator(const WedgeIterator<OtherFace>& other) :
            mFace(other.face()), mIndex(other.index())
    {
    }

    reference operator*() const { return reference(mFace, mIndex); }

    reference operator[](difference_type n) const
    {
        return reference(
            mFace, static_cast<uint>(static_cast<difference_type>(mIndex) + n));
    }

    WedgeIterator& operator++()
    {
        ++mIndex;
        return *this;
    }

    WedgeIterator operator++(int)
    {
        auto tmp = *this;
        ++mIndex;
        return tmp;
    }

    WedgeIterator& operator--()
    {
        --mIndex;
        return *this;
    }

    WedgeIterator operator--(int)
    {
        auto tmp = *this;
        --mIndex;
        return tmp;
    }

    WedgeIterator& operator+=(difference_type n)
    {
        mIndex = static_cast<uint>(static_cast<difference_type>(mIndex) + n);
        return *this;
    }

    WedgeIterator& operator-=(difference_type n)
    {
        mIndex = static_cast<uint>(static_cast<difference_type>(mIndex) - n);
        return *this;
    }

    friend WedgeIterator operator+(WedgeIterator it, difference_type n)
    {
        return it += n;
    }

    friend WedgeIterator operator+(difference_type n, WedgeIterator it)
    {
        return it += n;
    }

    friend WedgeIterator operator-(WedgeIterator it, difference_type n)
    {
        return it -= n;
    }

    const FaceType& face() const { return mFace; }

    uint index() const { return mIndex; }

    template<typename OtherFace>
    friend difference_type operator-(
        const WedgeIterator&            a,
        const WedgeIterator<OtherFace>& b)
    {
        assert(a.face() == b.face());
        return static_cast<difference_type>(a.index()) -
               static_cast<difference_type>(b.index());
    }

    template<typename OtherFace>
    bool operator==(const WedgeIterator<OtherFace>& other) const
    {
        return mIndex == other.index() && mFace == other.face();
    }

    template<typename OtherFace>
    auto operator<=>(const WedgeIterator<OtherFace>& other) const
    {
        if (auto cmp = mFace <=> other.face(); cmp != 0) {
            return cmp;
        }
        return mIndex <=> other.index();
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGES_WEDGE_ITERATOR_H
