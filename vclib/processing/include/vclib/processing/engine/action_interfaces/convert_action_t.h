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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_CONVERT_ACTION_T_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_CONVERT_ACTION_T_H

#include "convert_action.h"

#include <vclib/concepts/mesh.h>

namespace vcl::proc {

template<MeshConcept Mesh>
class ConvertActionT : public ConvertAction
{
public:
    using MeshType = Mesh;

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    /**
     * @brief Converts a mesh from the templated MeshType of the action to
     * a target mesh type.
     *
     * The converted mesh is returned in a std::pair. The first element
     * is the MeshTypeId of the output mesh, and the second element is the
     * output mesh itself, contained in a std::any object.
     *
     * @param inputMesh
     * @param log
     */
    virtual std::pair<MeshTypeId, std::any> convert(
        const MeshType&                  inputMesh,
        AbstractLogger&                  log = logger()) const = 0;

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    MeshTypeId meshType() const final { return meshTypeId<MeshType>(); }

};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_CONVERT_ACTION_T_H
