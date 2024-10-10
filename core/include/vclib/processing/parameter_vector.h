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

#ifndef VCL_PROCESSING_PARAMETER_VECTOR_H
#define VCL_PROCESSING_PARAMETER_VECTOR_H

#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include "parameters/parameter.h"

namespace vcl::proc {

class ParameterVector : public PolymorphicObjectVector<Parameter>
{
public:
    std::shared_ptr<const Parameter> get(const std::string& name) const
    {
        for (const auto& parameter : *this) {
            if (parameter->name() == name) {
                return parameter;
            }
        }

        return nullptr;
    }

    std::shared_ptr<Parameter> get(const std::string& name)
    {
        for (auto& parameter : *this) {
            if (parameter->name() == name) {
                return parameter;
            }
        }

        return nullptr;
    }
};

using OutputValues = ParameterVector;

} // namespace vcl::proc

#endif // VCL_PROCESSING_PARAMETER_VECTOR_H
