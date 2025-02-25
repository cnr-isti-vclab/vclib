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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IO_MESH_ACTION_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IO_MESH_ACTION_H

#include "io_action.h"

#include <vclib/processing/engine/parameter_vector.h>

#include <vclib/algorithms/mesh/update.h>
#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/io/file_format.h>
#include <vclib/io/file_info.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl::proc {

template<MeshConcept Mesh>
class IOMeshAction : public IOAction
{
public:
    using MeshType = Mesh;

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    [[nodiscard]] virtual std::shared_ptr<Action> clone() const = 0;

    virtual std::string name() const = 0;

    // From IOAction class

    virtual IOSupport ioSupport() const = 0;

    /**
     * @brief Returns a vector of file formats and their capabilities.
     *
     * Each file format is defined by a list of extensions (all the possible
     * extensions that a file format could have) and a description.
     *
     * The capabilities of the file format are defined by the MeshInfo
     * class, and they are used to determine the information that can be
     * stored or loaded from the file.
     *
     * @return the vectir of file formats supported by the action and their
     * capabilities
     */
    virtual std::vector<std::pair<FileFormat, MeshInfo>> supportedMeshFormats()
        const = 0;

    /**
     * @brief Returns the parameters to load the mesh.
     *
     * By default, the load function has no parameters.
     *
     * You should override this method if your load function requires
     * parameters.
     *
     * @param[in] format: the file format for which the parameters are requested
     *
     * @return The parameters for loading the mesh with the given file format.
     */
    virtual ParameterVector parametersLoad(const FileFormat& format) const
    {
        return ParameterVector();
    }

    /**
     * @brief Returns the parameters to save the mesh.
     *
     * By default, the save function has no parameters.
     *
     * You should override this method if your save function requires
     * parameters.
     *
     * @param[in] format: the file format for which the parameters are requested
     *
     * @return The parameters for saving the mesh with the given file format.
     */
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

    Type type() const final { return Type::IO_MESH_ACTION; }

    MeshTypeId meshType() const final { return meshTypeId<MeshType>(); }

    std::string identifier() const final
    {
        return identifierFromName(name() + " " + vcl::meshTypeName<MeshType>());
    }

    std::vector<FileFormat> supportedFormats() const final
    {
        std::vector<FileFormat> formats;
        for (const auto& [f, _] : supportedMeshFormats()) {
            formats.push_back(f);
        }
        return formats;
    }

    MeshType load(
        const std::string&     filename,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        MeshInfo info;
        FileFormat format(FileInfo::extension(filename));
        auto     mesh = load(filename, format, parameters, info, log);
        return mesh;
    }

    MeshType load(const std::string& filename, AbstractLogger& log = logger())
        const
    {
        FileFormat format(FileInfo::extension(filename));
        return load(filename, parametersLoad(format), log);
    }

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

    void save(
        const std::string& filename,
        const MeshType&    mesh,
        const MeshInfo&    info,
        AbstractLogger&    log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, mesh, info, parametersSave(format), log);
    }

    void save(
        const std::string&     filename,
        const MeshType&        mesh,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, mesh, formatCapability(format), parameters, log);
    }

    void save(
        const std::string& filename,
        const MeshType&    mesh,
        AbstractLogger&    log = logger()) const
    {
        FileFormat format(FileInfo::extension(filename));
        save(filename, mesh, parametersSave(format), log);
    }

protected:
    void postLoad(MeshType& mesh, const MeshInfo& loadedInfo) const
    {
        if constexpr (HasFaces<MeshType>) {
            if (!loadedInfo.hasFaceNormals()) {
                vcl::updatePerFaceNormals(mesh);
            }
            if (!loadedInfo.hasVertexNormals()) {
                vcl::updatePerVertexNormalsFromFaceNormals(mesh);
            }
        }
        vcl::updateBoundingBox(mesh);
    }

    MeshInfo formatCapability(const FileFormat& format) const
    {
        for (const auto& [f, info] : supportedMeshFormats()) {
            if (f == format) {
                return info;
            }
        }
        return MeshInfo();
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IO_MESH_ACTION_H
