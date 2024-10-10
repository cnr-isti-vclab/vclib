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

#ifndef VCL_EXT_VCG_TYPE_MAPPING_H
#define VCL_EXT_VCG_TYPE_MAPPING_H

#include "concepts.h"

#include <vclib/space/core/point.h>

#include <vcg/space/point2.h>
#include <vcg/space/point3.h>
#include <vcg/space/point4.h>

namespace vcl::vc {

// default mapping: identity
template<typename T>
struct TypeMapping
{
    using type = T;
};

/**** vcg -> vcl mapping ****/

// Point2
template<vc::Point2Concept T>
struct TypeMapping<T>
{
    using type = vcl::Point2<typename T::ScalarType>;
};

// Point3
template<vc::Point3Concept T>
struct TypeMapping<T>
{
    using type = vcl::Point3<typename T::ScalarType>;
};

// Point4
template<vc::Point4Concept T>
struct TypeMapping<T>
{
    using type = vcl::Point4<typename T::ScalarType>;
};

/**** vcl -> vcg mapping ****/

template<vcl::Point2Concept T>
struct TypeMapping<T>
{
    using type = vcg::Point2<typename T::ScalarType>;
};

template<vcl::Point3Concept T>
struct TypeMapping<T>
{
    using type = vcg::Point3<typename T::ScalarType>;
};

template<vcl::Point4Concept T>
struct TypeMapping<T>
{
    using type = vcg::Point4<typename T::ScalarType>;
};

// conversion functions

// unrecognized type
template<typename T>
auto fromVCG(const T& object)
{
    return object;
}

template<vc::Point2Concept T>
auto fromVCG(const T& vcgPoint)
{
    using VCLPoint = TypeMapping<T>::type;
    return VCLPoint(vcgPoint.X(), vcgPoint.Y());
}

template<vc::Point3Concept T>
auto fromVCG(const T& vcgPoint)
{
    using VCLPoint = TypeMapping<T>::type;
    return VCLPoint(vcgPoint.X(), vcgPoint.Y(), vcgPoint.Z());
}

template<vc::Point4Concept T>
auto fromVCG(const T& vcgPoint)
{
    using VCLPoint = TypeMapping<T>::type;
    return VCLPoint(vcgPoint.V(0), vcgPoint.V(1), vcgPoint.V(2), vcgPoint.V(3));
}

// unrecognized type
template<typename T>
auto toVCG(const T& object)
{
    return object;
}

template<vcl::Point2Concept T>
auto toVCG(const T& vclPoint)
{
    using VCGPoint = TypeMapping<T>::type;
    return VCGPoint(vclPoint.x(), vclPoint.y());
}

template<vcl::Point3Concept T>
auto toVCG(const T& vclPoint)
{
    using VCGPoint = TypeMapping<T>::type;
    return VCGPoint(vclPoint.x(), vclPoint.y(), vclPoint.z());
}

template<vcl::Point4Concept T>
auto toVCG(const T& vclPoint)
{
    using VCGPoint = TypeMapping<T>::type;
    return VCGPoint(vclPoint.x(), vclPoint.y(), vclPoint.z(), vclPoint.w());
}

} // namespace vcl::vc

#endif // VCL_EXT_VCG_TYPE_MAPPING_H
