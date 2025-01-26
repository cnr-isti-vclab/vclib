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

#ifndef VCL_SPACE_CORE_TEX_COORD_H
#define VCL_SPACE_CORE_TEX_COORD_H

#include "point.h"

namespace vcl {

/**
 * @brief The TexCoord class represents a 2-dimensional texture coordinate
 * containing two scalar values.
 *
 * The TexCoord class template represents a 2-dimensional texture coordinate
 * containing two scalar values. The scalar type of the texture coordinates is a
 * template parameter of the class.
 *
 * The class is a specialization of the Point2 class template, where the two
 * components of the texture coordinate are named `u` and `v`. The class adds
 * setter member functions for setting the `u` and `v` components of the texture
 * coordinate, with an additional assert that the values are in the range [0,
 * 1].
 *
 * @tparam Scalar: The scalar type of the texture coordinate components.
 *
 * @ingroup space_core
 */
template<typename Scalar>
class TexCoord : public Point2<Scalar>
{
    using Base = Point2<Scalar>;
public:
    using Base::Base;

    // inherit Base operators
    using Base::operator+;
    using Base::operator-;
    using Base::operator*;
    using Base::operator/;
    using Base::operator+=;
    using Base::operator-=;
    using Base::operator*=;
    using Base::operator/=;

    Scalar u() const { return Base::x(); }

    Scalar v() const { return Base::y(); }

    Scalar& u() { return Base::x(); }

    Scalar& v() { return Base::y(); }

    void setU(Scalar s)
    {
        assert(s >= 0 && s <= 1);
        Base::x() = s;
    }

    void setV(Scalar s)
    {
        assert(s >= 0 && s <= 1);
        Base::y() = s;
    }

    void set(Scalar u, Scalar v)
    {
        setU(u);
        setV(v);
    }
};

/* Specialization Aliases */

using TexCoordi = TexCoord<int>;
using TexCoordf = TexCoord<float>;
using TexCoordd = TexCoord<double>;

} // namespace vcl

#endif // VCL_SPACE_CORE_TEX_COORD_H
