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

#ifndef VCL_PROCESSING_ACTIONS_INTERFACES_CREATE_FILTER_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_INTERFACES_CREATE_FILTER_MESH_ACTION_H

#include "filter_mesh_action.h"

namespace vcl::proc {

class CreateFilterMeshAction : public FilterMeshAction
{
public:
    uint numberInputMeshes() const final { return 0; }

    vcl::BitSet<short> supportedInputMeshTypes(uint) const final
    {
        vcl::BitSet<short> bs;
        bs.reset();
        return bs;
    }

    uint numberInputOutputMeshes() const final { return 0; }

    vcl::BitSet<short> supportedInputOutputMeshTypes(uint) const final
    {
        return supportedInputMeshTypes(0);
    }

    vcl::BitSet<uint> categories() const
    {
        return vcl::BitSet<uint>({CREATE});
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_INTERFACES_CREATE_FILTER_MESH_ACTION_H
