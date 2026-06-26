// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_VCG_TYPE_MAPPING_H
#define VCL_VCG_TYPE_MAPPING_H

#include "concepts.h"

#include <vclib/space/core.h>

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

#endif // VCL_VCG_TYPE_MAPPING_H
