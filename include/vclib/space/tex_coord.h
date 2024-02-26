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

#ifndef VCL_SPACE_TEX_COORD_H
#define VCL_SPACE_TEX_COORD_H

#include "point.h"

namespace vcl {

template<typename Scalar>
class TexCoord
{
    template<typename S>
    friend class TexCoord;

    Point2<Scalar> coord;

public:
    using ScalarType = Scalar;

    TexCoord() = default;

    TexCoord(const Scalar& s1, const Scalar& s2) : coord(s1, s2) {}

    TexCoord(const Point2<Scalar>& p) : coord(p) {}

    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same<Scalar, S>::value) {
            return *this;
        }
        else {
            TexCoord<S> tmp;
            tmp.coord = coord.template cast<S>();
            return tmp;
        }
    }

    Scalar u() const { return coord.x(); }

    Scalar v() const { return coord.y(); }

    Scalar& u() { return coord.x(); }

    Scalar& v() { return coord.y(); }

    void setU(Scalar s)
    {
        assert(s >= 0 && s <= 1);
        coord.x() = s;
    }

    void setV(Scalar s)
    {
        assert(s >= 0 && s <= 1);
        coord.y() = s;
    }

    void set(Scalar u, Scalar v)
    {
        setU(u);
        setV(v);
    }

    // operators
    Scalar& operator()(uint i) { return coord[i]; }

    const Scalar& operator()(uint i) const { return coord[i]; }

    Scalar& operator[](uint i) { return coord[i]; }

    const Scalar& operator[](uint i) const { return coord[i]; }

    bool operator==(const TexCoord& t1) const = default;
};

/* Specialization Aliases */

using TexCoordi = TexCoord<int>;
using TexCoordf = TexCoord<float>;
using TexCoordd = TexCoord<double>;

} // namespace vcl

#endif // VCL_SPACE_TEX_COORD_H
