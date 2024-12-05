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

#ifndef VCL_PROCESSING_MESHES_MESH_I_H
#define VCL_PROCESSING_MESHES_MESH_I_H

#include <vclib/concepts/mesh/mesh_concept.h>

namespace vcl::proc {

struct MeshIType
{
    enum Enum {
        TRI_MESH,
        POLY_MESH,

        COUNT
    };
};

class MeshI
{
public:
    MeshI()          = default;
    virtual ~MeshI() = default;

    virtual MeshIType::Enum type() const = 0;

    virtual std::string typeName() const = 0;

    template<MeshConcept MeshType>
    bool is() const
    {
        return dynamic_cast<const MeshType*>(this) != nullptr;
    }

    template<MeshConcept MeshType>
    MeshType& as()
    {
        return dynamic_cast<MeshType&>(*this);
    }

    template<MeshConcept MeshType>
    const MeshType& as() const
    {
        return dynamic_cast<const MeshType&>(*this);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MESHES_MESH_I_H
