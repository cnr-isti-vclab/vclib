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

#ifndef VCL_PROCESSING_ENGINE_PARAMETERS_ENUM_PARAMETER_H
#define VCL_PROCESSING_ENGINE_PARAMETERS_ENUM_PARAMETER_H

#include "parameter.h"

#include <vclib/concepts/ranges/range.h>

namespace vcl::proc {

class EnumParameter : public Parameter
{
    std::vector<std::string> mEnumValues;
    vcl::BitSet32            mEnabled;

public:
    EnumParameter(
        const std::string&              name,
        uint                            value,
        const std::vector<std::string>& enumValues,
        vcl::BitSet32                   enabled     = vcl::BitSet32().set(),
        const std::string&              description = "",
        const std::string&              tooltip     = "",
        const std::string&              category    = "") :
            Parameter(name, 0u, description, tooltip, category),
            mEnumValues(enumValues), mEnabled(enabled)
    {
        if (mEnumValues.size() >= 32) {
            throw std::runtime_error(
                "The number of enum values exceeds the maximum allowed value "
                "of 32.");
        }
        setUintValue(value);
    }

    ParameterType type() const override { return ParameterType::ENUM; }

    std::shared_ptr<Parameter> clone() const override
    {
        return std::make_shared<EnumParameter>(*this);
    }

    void setUintValue(uint value) override
    {
        checkEnumValueEnabled(value);
        Parameter::setUintValue(value);
    }

    const std::vector<std::string>& enumValues() const { return mEnumValues; }

    uint enumUintValue(const std::string& value) const
    {
        auto it = std::find(mEnumValues.begin(), mEnumValues.end(), value);
        if (it == mEnumValues.end())
            throw std::runtime_error("Invalid enum string value: " + value);
        return it - mEnumValues.begin();
    }

    const std::string& enumValue(uint value) const
    {
        checkEnumValue(value);
        return mEnumValues[value];
    }

    bool isEnabled(uint value) const
    {
        checkEnumValue(value);
        return mEnabled[value];
    }

    bool isEnabled(const std::string& value) const
    {
        return isEnabled(enumUintValue(value));
    }

    const std::string& enumValue() const { return mEnumValues[uintValue()]; }

    void setEnumValue(const std::string& value)
    {
        setUintValue(enumUintValue(value));
    }

private:
    void checkEnumValue(uint value) const
    {
        if (value >= mEnumValues.size())
            throw std::runtime_error(
                "Invalid enum value: " + std::to_string(value) +
                "; expected value in [0, " +
                std::to_string(mEnumValues.size()) + ")");
    }

    void checkEnumValueEnabled(uint value) const
    {
        checkEnumValue(value);
        if (mEnabled[value] == false)
            throw std::runtime_error(
                "Enum value " + std::to_string(value) + " is disabled.");
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_PARAMETERS_ENUM_PARAMETER_H
