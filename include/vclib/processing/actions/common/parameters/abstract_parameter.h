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

#ifndef VCL_PROCESSING_ACTIONS_COMMON_PARAMETERS_ABSTRACT_PARAMETER_H
#define VCL_PROCESSING_ACTIONS_COMMON_PARAMETERS_ABSTRACT_PARAMETER_H

#include <any>
#include <memory>
#include <string>

#include <vclib/processing/settings.h>
#include <vclib/space/color.h>
#include <vclib/space/point.h>

namespace vcl {

struct ParameterType
{
    enum Enum {
        INT,
        SCALAR,
        BOOL,
        STRING,
        COLOR,
        POINT3,

        COUNT
    };
};

class AbstractParameter {
    std::string mName;
    std::any    mValue;
    std::string mDescription;
    std::string mToolTip;
    std::string mCategory;

public:
    using Scalar = ProcScalarType;

    AbstractParameter() = default;

    virtual ~AbstractParameter() = default;

    virtual ParameterType::Enum type() const = 0;

    virtual std::shared_ptr<AbstractParameter> clone() const = 0;

    const std::string& name() const { return mName; }

    template<typename ValueType>
    const ValueType& value() const
    {
        return std::any_cast<ValueType>(mValue);
    }

    const std::string& description() const { return mDescription; }

    const std::string& tooltip() const { return mToolTip; }

    const std::string& category() const { return mCategory; }

    bool& boolValue() { return std::any_cast<bool&>(mValue); }

    bool boolValue() const { return std::any_cast<bool>(mValue); }

    int& intValue() { return std::any_cast<int&>(mValue); }

    int intValue() const { return std::any_cast<int>(mValue); }

    Scalar& scalarValue() { return std::any_cast<Scalar&>(mValue); }

    Scalar scalarValue() const { return std::any_cast<Scalar>(mValue); }

    std::string& stringValue() { return std::any_cast<std::string&>(mValue); }

    const std::string& stringValue() const
    {
        return std::any_cast<const std::string&>(mValue);
    }

    vcl::Color& colorValue() { return std::any_cast<vcl::Color&>(mValue); }

    const vcl::Color& colorValue() const
    {
        return std::any_cast<const vcl::Color&>(mValue);
    }

    Point3<ProcScalarType>& point3Value()
    {
        return std::any_cast<Point3<ProcScalarType>&>(mValue);
    }

    const Point3<ProcScalarType>& point3Value() const
    {
        return std::any_cast<const Point3<ProcScalarType>&>(mValue);
    }

protected:
    template<typename ValueType>
    AbstractParameter(
        const std::string& name,
        const ValueType& value,
        const std::string& description,
        const std::string& tooltip,
        const std::string& category) :
            mName(name), mValue(value),
            mDescription(description), mToolTip(tooltip), mCategory(category)
    {
    }
};

} // namespace vcl

#endif // VCL_PROCESSING_ACTIONS_COMMON_PARAMETERS_ABSTRACT_PARAMETER_H
