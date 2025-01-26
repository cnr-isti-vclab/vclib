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

#ifndef VCL_SPACE_CORE_TEX_COORD_INDEXED_H
#define VCL_SPACE_CORE_TEX_COORD_INDEXED_H

#include "tex_coord.h"

namespace vcl {

/**
 * @brief The TexCoordIndexed class represents a texture coordinate with an
 * index.
 *
 * The TexCoordIndexed class represents a texture coordinate with an index. The
 * class is a specialization of the TexCoord class template, where the two
 * components of the texture coordinate are named `u` and `v`. The class adds
 * an index member variable that represents the index of the texture coordinate.
 *
 * @tparam Scalar: The scalar type of the texture coordinate components.
 *
 * @ingroup space_core
 */
template<typename Scalar>
class TexCoordIndexed : public TexCoord<Scalar>
{
    using Base = TexCoord<Scalar>;

    ushort mIndex = 0;

public:
    using Base::Base;

    using Base::operator();
    using Base::operator[];

    TexCoordIndexed() = default;

    TexCoordIndexed(const Base& base, ushort index) : Base(base), mIndex(index)
    {
    }

    TexCoordIndexed(const Scalar& s1, const Scalar& s2, ushort index) :
            Base(s1, s2), mIndex(index)
    {
    }

    TexCoordIndexed(const Point2<Scalar>& p, ushort index) :
            Base(p), mIndex(index)
    {
    }

    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same<Scalar, S>::value) {
            return *this;
        }
        else {
            TexCoordIndexed<S> tmp = Base::template cast<S>();
            tmp.mIndex = mIndex;
            return tmp;
        }
    }

    ushort index() const { return mIndex; }

    ushort& index() { return mIndex; }

    void serialize(std::ostream& os) const
    {
        Base::serialize(os);
        vcl::serialize(os, mIndex);
    }

    void deserialize(std::istream& is)
    {
        Base::deserialize(is);
        vcl::deserialize(is, mIndex);
    }

    // operators
    auto operator<=>(const TexCoordIndexed& t1) const = default;
};

} // namespace vcl

#endif // VCL_SPACE_CORE_TEX_COORD_INDEXED_H
