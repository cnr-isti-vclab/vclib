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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_H

#include "action_manager/manager.h"

namespace vcl::proc {

class ActionManager
{
public:
    // add action functions

    static void add(const std::shared_ptr<Action>& action)
    {
        instance().add(action);
    }

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    static void add(R&& actions)
    {
        instance().add(std::forward<R>(actions));
    }

    // load image

    static std::vector<FileFormat> loadImageFormats()
    {
        return instance().loadImageFormats();
    }

    static std::shared_ptr<ImageIOAction> loadImageAction(FileFormat fmt)
    {
        return instance().loadImageAction(fmt);
    }

    // save image

    static std::vector<FileFormat> saveImageFormats()
    {
        return instance().saveImageFormats();
    }

    static std::shared_ptr<ImageIOAction> saveImageAction(FileFormat fmt)
    {
        return instance().saveImageAction(fmt);
    }

    // load mesh

    static std::vector<FileFormat> loadMeshFormats(MeshTypeId mt)
    {
        return instance().loadMeshFormats(mt);
    }

    template<typename MeshType>
    static std::vector<FileFormat> loadMeshFormats()
    {
        return instance().loadMeshFormats<MeshType>();
    }

    template<MeshTypeId MESH>
    static std::vector<FileFormat> loadMeshFormats()
    {
        return instance().loadMeshFormats<GetMeshType<MESH>>();
    }

    static std::vector<FileFormat> loadMeshFormats()
    {
        return instance().loadMeshFormats();
    }

    static std::shared_ptr<IOAction> loadMeshAction(
        FileFormat fmt,
        MeshTypeId mt)
    {
        return instance().loadMeshAction(fmt, mt);
    }

    template<typename MeshType>
    static std::shared_ptr<MeshIOAction<MeshType>> loadMeshAction(
        FileFormat fmt)
    {
        return instance().loadMeshAction<MeshType>(fmt);
    }

    template<MeshTypeId MESH>
    static std::shared_ptr<IOAction> loadMeshAction(FileFormat fmt)
    {
        return instance().loadMeshAction<GetMeshType<MESH>>(fmt);
    }

    // save mesh

    static std::vector<FileFormat> saveMeshFormats(MeshTypeId mt)
    {
        return instance().saveMeshFormats(mt);
    }

    template<typename MeshType>
    static std::vector<FileFormat> saveMeshFormats()
    {
        return instance().saveMeshFormats<MeshType>();
    }

    template<MeshTypeId MESH>
    static std::vector<FileFormat> saveMeshFormats()
    {
        return instance().saveMeshFormats<GetMeshType<MESH>>();
    }

    static std::vector<FileFormat> saveMeshFormats()
    {
        return instance().saveMeshFormats();
    }

    static std::shared_ptr<IOAction> saveMeshAction(
        FileFormat fmt,
        MeshTypeId mt)
    {
        return instance().saveMeshAction(fmt, mt);
    }

    template<typename MeshType>
    static std::shared_ptr<MeshIOAction<MeshType>> saveMeshAction(
        FileFormat fmt)
    {
        return instance().saveMeshAction<MeshType>(fmt);
    }

    template<MeshTypeId MESH>
    static auto saveMeshAction(FileFormat fmt)
    {
        return instance().saveMeshAction<GetMeshType<MESH>>(fmt);
    }

    // filter

    static std::shared_ptr<Action> filterAction(
        const std::string& name,
        MeshTypeId         mt)
    {
        return instance().filterAction(name, mt);
    }

    template<typename MeshType>
    static std::shared_ptr<FilterAction<MeshType>> filterAction(
        const std::string& name)
    {
        return instance().filterAction<MeshType>(name);
    }

    template<MeshTypeId MESH>
    static auto filteraction(const std::string& name)
    {
        return instance().filterAction<GetMeshType<MESH>>(name);
    }

private:
    static detail::Manager& instance()
    {
        static detail::Manager instance;
        return instance;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_H
