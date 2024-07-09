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

#ifndef VCL_PROCESSING_PARAMETERS_MESH_PARAMETER_H
#define VCL_PROCESSING_PARAMETERS_MESH_PARAMETER_H

#include "parameter.h"

#include <vclib/concepts/ranges/range.h>

namespace vcl::proc {

class MeshParameter : public Parameter
{
    std::vector<std::pair<std::string, bool>> mMeshValues;

public:
    MeshParameter(
        const std::string& name        = "",
        const std::string& description = "",
        const std::string& tooltip     = "",
        const std::string& category    = "") :
            Parameter(name, UINT_NULL, description, tooltip, category)
    {
    }

    ParameterType::Enum type() const override { return ParameterType::MESH; }

    std::shared_ptr<Parameter> clone() const override
    {
        return std::make_shared<MeshParameter>(*this);
    }

    void setUintValue(uint value) override
    {
        checkMeshValue(value);
        Parameter::setUintValue(value);
    }

    const std::vector<std::pair<std::string, bool>>& meshValues() const
    {
        return mMeshValues;
    }

    void setMeshValues(
        const std::vector<std::pair<std::string, bool>>& meshValues)
    {
        mMeshValues = meshValues;
    }

private:
    void checkMeshValue(uint value) const
    {
        if (value >= mMeshValues.size()) {
            throw std::runtime_error(
                "Invalid mesh value: " + std::to_string(value) +
                "; expected value in [0, " +
                std::to_string(mMeshValues.size()) + ")");
        }
        if (!mMeshValues[value].second) {
            throw std::runtime_error(
                "Invalid mesh value: Mesh " + mMeshValues[value].first + " (" +
                std::to_string(value) + ") is disabled");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_PARAMETERS_MESH_PARAMETER_H
