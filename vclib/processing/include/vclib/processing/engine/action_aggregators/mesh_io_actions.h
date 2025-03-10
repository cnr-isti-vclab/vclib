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

#ifndef VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_MESH_IO_ACTIONS_H
#define VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_MESH_IO_ACTIONS_H

#include "fill_actions.h"

#include <vclib/processing/engine/action_interfaces/mesh_io_action_t.h>

namespace vcl::proc {

class MeshIOActions : public MeshIOAction
{
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    std::array<std::shared_ptr<MeshIOAction>, MESH_TYPE_NUMBER> mMeshIOActions;
    uint mFirstMeshType = MESH_TYPE_NUMBER;

public:
    template<template<typename> typename Act>
    void fillWithSupportedMeshTypes()
    {
        detail::fillWithSupportedMeshTypes<Act>(mMeshIOActions, mFirstMeshType);
    }

    std::string name() const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->name();
    }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }

    IOSupport ioSupport() const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->ioSupport();
    }

    std::vector<std::pair<FileFormat, MeshInfo>> supportedMeshFormats()
        const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->supportedMeshFormats();
    }

    ParameterVector parametersLoad(const FileFormat& format) const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->parametersLoad(format);
    }

    ParameterVector parametersSave(const FileFormat& format) const final
    {
        checkActionHasBeenFilled();
        return mMeshIOActions[mFirstMeshType]->parametersSave(format);
    }

    BitSet32 supportedMeshTypes() const
    {
        checkActionHasBeenFilled();
        BitSet32 bitset;
        for (uint i = 0; i < MESH_TYPE_NUMBER; i++) {
            if (mMeshIOActions[i] != nullptr) {
                bitset[i] = true;
            }
        }
        return bitset;
    }

    template<MeshConcept MeshType>
    std::shared_ptr<MeshIOActionT<MeshType>> action() const
    {
        checkActionHasBeenFilled();
        checkActionForMeshType<MeshType>();
        return std::dynamic_pointer_cast<MeshIOActionT<MeshType>>(
            mMeshIOActions[toUnderlying(meshTypeId<MeshType>())]);
    }

    template<MeshConcept MeshType>
    MeshType load(
        const std::string&     filename,
        const FileFormat&      format,
        const ParameterVector& parameters,
        MeshInfo&              loadedInfo,
        AbstractLogger&        log = logger()) const
    {
        checkActionForMeshType<MeshType>(true);
        return action<MeshType>()->load(
            filename, format, parameters, loadedInfo, log);
    }

    template<MeshConcept MeshType>
    MeshType load(
        const std::string&     filename,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        MeshInfo   info;
        FileFormat format(FileInfo::extension(filename));
        auto mesh = load<MeshType>(filename, format, parameters, info, log);
        return mesh;
    }

    template<MeshConcept MeshType>
    MeshType load(const std::string& filename, AbstractLogger& log = logger())
        const
    {
        FileFormat format(FileInfo::extension(filename));
        return load<MeshType>(filename, parametersLoad(format), log);
    }

    template<MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const FileFormat&      format,
        const MeshType&        mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        checkActionForMeshType<MeshType>(false, true);
        action<MeshType>()->save(filename, format, mesh, info, parameters, log);
    }

    template<MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, format, mesh, info, parameters, log);
    }

    template<MeshConcept MeshType>
    void save(
        const std::string& filename,
        const MeshType&    mesh,
        const MeshInfo&    info,
        AbstractLogger&    log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, mesh, info, parametersSave(format), log);
    }

    template<MeshConcept MeshType>
    void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, mesh, formatCapability(format), parameters, log);
    }

    template<MeshConcept MeshType>
    void save(
        const std::string& filename,
        const MeshType&    mesh,
        AbstractLogger&    log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, mesh, parametersSave(format), log);
    }

private:
    void checkActionHasBeenFilled() const
    {
        if (mFirstMeshType >= MESH_TYPE_NUMBER) {
            throw std::runtime_error("The action has not been filled.");
        }
    }

    template<MeshConcept MeshType>
    void checkActionForMeshType(bool load = false, bool save = false) const
    {
        checkActionHasBeenFilled();
        checkMeshTypeId<MeshType>();
        uint id = toUnderlying(meshTypeId<MeshType>());
        if (mMeshIOActions[id] == nullptr) {
            throw std::runtime_error(
                "The action cannot be instantiated for the given MeshType.");
        }
        if (load && mMeshIOActions[id]->ioSupport() == IOSupport::SAVE) {
            throw std::runtime_error(
                "The action does not support loading meshes.");
        }
        if (save && mMeshIOActions[id]->ioSupport() == IOSupport::LOAD) {
            throw std::runtime_error(
                "The action does not support saving meshes.");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_AGGREGATORS_MESH_IO_ACTIONS_H
