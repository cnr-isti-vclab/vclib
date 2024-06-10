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

#ifndef VCL_PROCESSING_ACTIONS_COMMON_PARAMETERS_PARAMETER_H
#define VCL_PROCESSING_ACTIONS_COMMON_PARAMETERS_PARAMETER_H

#include <any>
#include <memory>
#include <string>

#include <vclib/processing/settings.h>
#include <vclib/space/color.h>
#include <vclib/space/point.h>

namespace vcl::proc {

struct ParameterType
{
    enum Enum {
        // native parameters
        INT,
        SCALAR,
        BOOL,
        STRING,
        COLOR,
        POINT3,

        // special parameters - they store native parameters, but they add
        // additional information or functionalities
        ENUM, // enum is an int that can be converted to a string

        COUNT
    };
};

class Parameter {
    std::string mName;
    std::any    mValue;
    std::string mDescription;
    std::string mToolTip;
    std::string mCategory;

public:
    using Scalar = ProcScalarType;

    Parameter() = default;

    virtual ~Parameter() = default;

    virtual ParameterType::Enum type() const = 0;

    virtual std::shared_ptr<Parameter> clone() const = 0;

    const std::string& name() const { return mName; }

    template<typename ValueType>
    const ValueType& value() const
    {
        return std::any_cast<ValueType>(mValue);
    }

    const std::string& description() const { return mDescription; }

    const std::string& tooltip() const { return mToolTip; }

    const std::string& category() const { return mCategory; }

    void setBoolValue(bool v)
    {
        checkParameterType(ParameterType::BOOL);
        std::any_cast<bool&>(mValue) = v;
    }

    bool boolValue() const
    {
        checkParameterType(ParameterType::BOOL);
        return std::any_cast<bool>(mValue);
    }

    virtual void setIntValue(int v)
    {
        checkParameterType(ParameterType::INT);
        std::any_cast<int&>(mValue) = v;
    }

    int intValue() const
    {
        checkParameterType(ParameterType::INT);
        return std::any_cast<int>(mValue);
    }

    virtual void setScalarValue(Scalar v)
    {
        checkParameterType(ParameterType::SCALAR);
        std::any_cast<Scalar&>(mValue) = v;
    }

    Scalar scalarValue() const
    {
        checkParameterType(ParameterType::SCALAR);
        return std::any_cast<Scalar>(mValue);
    }

    virtual void setStringValue(const std::string& v)
    {
        checkParameterType(ParameterType::STRING);
        std::any_cast<std::string&>(mValue) = v;
    }

    const std::string& stringValue() const
    {
        checkParameterType(ParameterType::STRING);
        return std::any_cast<const std::string&>(mValue);
    }

    void setColorValue(const vcl::Color& v)
    {
        checkParameterType(ParameterType::COLOR);
        std::any_cast<vcl::Color&>(mValue) = v;
    }

    const vcl::Color& colorValue() const
    {
        checkParameterType(ParameterType::COLOR);
        return std::any_cast<const vcl::Color&>(mValue);
    }

    void setPoint3Value(const Point3<ProcScalarType>& v)
    {
        checkParameterType(ParameterType::POINT3);
        std::any_cast<Point3<ProcScalarType>&>(mValue) = v;
    }

    const Point3<ProcScalarType>& point3Value() const
    {
        checkParameterType(ParameterType::POINT3);
        return std::any_cast<const Point3<ProcScalarType>&>(mValue);
    }

protected:
    template<typename ValueType>
    Parameter(
        const std::string& name,
        const ValueType& value,
        const std::string& description,
        const std::string& tooltip,
        const std::string& category) :
            mName(name), mValue(value),
            mDescription(description), mToolTip(tooltip), mCategory(category)
    {
    }

private:
    void checkParameterType(ParameterType::Enum t) const
    {
        if (nativeType() != t) {
            throw std::runtime_error("Invalid parameter type");
        }
    }

    ParameterType::Enum nativeType() const
    {
        ParameterType::Enum t = type();
        switch (t) {
        case vcl::proc::ParameterType::ENUM:
            return ParameterType::INT;
        default:
            return t;
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_COMMON_PARAMETERS_PARAMETER_H
