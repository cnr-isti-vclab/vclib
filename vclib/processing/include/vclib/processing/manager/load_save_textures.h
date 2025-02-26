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

#ifndef VCL_PROCESSING_MANAGER_LOAD_SAVE_TEXTURES_H
#define VCL_PROCESSING_MANAGER_LOAD_SAVE_TEXTURES_H

#include "action_manager.h"

namespace vcl::proc {

template<MeshConcept MeshType>
void saveTexturesUsingManager(const MeshType& mesh, const std::string& basePath)
{
    if constexpr (HasTextureImages<MeshType>) {
        for (const vcl::Texture& texture : mesh.textures()) {
            std::string ext = FileInfo::extension(texture.path());

            try {
                auto act = ActionManager::saveImageAction(ext);
                act->save(basePath + texture.path(), texture.image());
            }
            catch (const std::exception& e) {
                // todo: log error
                std::cerr << "Error saving texture: " << e.what() << std::endl;
            }
        }
    }
}

template<MeshConcept MeshType>
void loadTexturesUsingManager(MeshType& mesh, const std::string& basePath)
{
    if constexpr (HasTextureImages<MeshType>) {
        for (vcl::Texture& texture : mesh.textures()) {
            std::string ext = FileInfo::extension(texture.path());

            try {
                auto act        = ActionManager::loadImageAction(ext);
                texture.image() = act->load(basePath + texture.path());
            }
            catch (const std::exception& e) {
                // todo: log error
                std::cerr << "Error loading texture: " << e.what() << std::endl;
            }
        }
    }
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_LOAD_SAVE_TEXTURES_H
