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

#ifndef VCL_PROCESSING_FUNCTIONS_H
#define VCL_PROCESSING_FUNCTIONS_H

#include "action_manager.h"
#include "actions/common/file_format.h"

#include <vclib/io/file_info.h>

namespace vcl::proc {

template<MeshConcept MeshType>
void saveMeshTextures(
    const MeshType&    mesh,
    const std::string& filepath,
    ActionManager*     manager)
{
    for (const vcl::Texture& texture : mesh.textures()) {
        std::string ext = FileInfo::extension(texture.path());

        try {
            auto act = manager->saveImageAction(ext);
            act->save(filepath + texture.path(), texture.image());
        }
        catch (const std::exception& e) {
            // todo: log error
            std::cerr << "Error saving texture: " << e.what() << std::endl;
        }
    }
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_FUNCTIONS_H
