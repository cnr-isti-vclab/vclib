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

#ifndef VCL_PROCESSING_PARAMETERS_USCALAR_PARAMETER_H
#define VCL_PROCESSING_PARAMETERS_USCALAR_PARAMETER_H

#include "parameter.h"

namespace vcl::proc {

class UscalarParameter : public Parameter
{
public:
    UscalarParameter(
        const std::string& name,
        Scalar             value,
        const std::string& description = "",
        const std::string& tooltip     = "",
        const std::string& category    = "") :
            Parameter(name, 0.0, description, tooltip, category)
    {
        setScalarValue(value);
    }

    ParameterType type() const override { return ParameterType::USCALAR; }

    std::shared_ptr<Parameter> clone() const override
    {
        return std::make_shared<UscalarParameter>(*this);
    }

    void setScalarValue(Scalar value) override
    {
        checkScalarValue(value);
        Parameter::setScalarValue(value);
    }

private:
    void checkScalarValue(Scalar value) const
    {
        if (value < 0.0)
            throw std::runtime_error(
                "UscalarParameter: value cannot be negative");
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_PARAMETERS_USCALAR_PARAMETER_H
