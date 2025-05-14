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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_MESH_IO_ACTION_T_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_MESH_IO_ACTION_T_H

#include "mesh_io_action.h"

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/algorithms/mesh/update.h>
#include <vclib/io/file_info.h>

namespace vcl::proc {

template<MeshConcept Mesh>
class MeshIOActionT : public MeshIOAction
{
public:
    using MeshType = Mesh;

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    // From IOAction class

    virtual IOSupport ioSupport() const = 0;

    // From MeshIOAction class

    virtual std::vector<std::pair<FileFormat, MeshInfo>> supportedMeshFormats()
        const = 0;

    virtual ParameterVector parametersLoad(const FileFormat& format) const
    {
        return ParameterVector();
    }

    virtual ParameterVector parametersSave(const FileFormat& format) const
    {
        return ParameterVector();
    }

    /**
     * @brief Loads a mesh from the given file.
     *
     * This member function must be implemented by the derived classes if the
     * @ref ioSupport member function returns IOSupport::LOAD or
     * IOSupport::BOTH.
     *
     * @param[in] filename: the file to read from
     * @param[in] format: the file format of the file to read from
     * @param[in] parameters: the parameters for loading the mesh
     * @param[out] loadedInfo: the information loaded from the file
     * @param[in] log: the logger to use
     * @return the mesh loaded from the file
     */
    virtual MeshType load(
        const std::string&     filename,
        const FileFormat&      format,
        const ParameterVector& parameters,
        vcl::MeshInfo&         loadedInfo,
        AbstractLogger&        log = logger()) const
    {
        if (ioSupport() == IOSupport::SAVE) {
            throw std::runtime_error(
                "The action " + name() + " does not support loading meshes.");
            return MeshType();
        }
        // This should never be reached - the action declared that is able to
        // load images, but it does not implement the load method.
        assert(0);
        return MeshType();
    };

    /**
     * @brief Saves the given mesh to the given file.
     *
     * This member function must be implemented by the derived classes if the
     * @ref ioSupport member function returns IOSupport::SAVE or
     * IOSupport::BOTH.
     *
     * @param[in] filename: the file to write to
     * @param[in] format: the file format of the file to write to
     * @param[in] mesh: the mesh to save
     * @param[in] info: the information of the mesh to save in the file
     * @param[in] parameters: the parameters for saving the mesh
     * @param[in] log: the logger to use
     */
    virtual void save(
        const std::string&     filename,
        const FileFormat&      format,
        const MeshType&        mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        if (ioSupport() == IOSupport::LOAD) {
            throw std::runtime_error(
                "The action " + name() + " does not support saving meshes.");
        }
        // This should never be reached - the action declared that is able to
        // save images, but it does not implement the save method.
        assert(0);
    }

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    MeshTypeId meshType() const final { return meshTypeId<MeshType>(); }

protected:
    void postLoad(MeshType& mesh, const MeshInfo& loadedInfo) const
    {
        if constexpr (HasFaces<MeshType>) {
            if (!loadedInfo.hasPerFaceNormal()) {
                vcl::updatePerFaceNormals(mesh);
            }
            if (!loadedInfo.hasPerVertexNormal()) {
                vcl::updatePerVertexNormalsFromFaceNormals(mesh);
            }
        }
        vcl::updateBoundingBox(mesh);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_MESH_IO_ACTION_T_H
