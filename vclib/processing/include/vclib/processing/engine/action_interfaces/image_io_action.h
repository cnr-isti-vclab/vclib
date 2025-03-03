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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IMAGE_IO_ACTION_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IMAGE_IO_ACTION_H

#include "io_action.h"

#include <vclib/io/file_format.h>
#include <vclib/space/core/image.h>

#include <vector>

namespace vcl::proc {

class ImageIOAction : public IOAction
{
public:
    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    virtual std::string name() const = 0;

    // From IOAction class

    virtual IOSupport ioSupport() const = 0;

    virtual std::vector<FileFormat> supportedFormats() const = 0;

    /**
     * @brief Loads an image from the given file.
     *
     * This member function must be implemented by the derived classes if the
     * @ref ioSupport member function returns IOSupport::LOAD or
     * IOSupport::BOTH.
     *
     * @param[in] filename: the file to read from
     * @param[in] log: the logger to use
     * @return the image loaded from the file
     */
    virtual Image load(
        const std::string& filename,
        AbstractLogger&    log = logger()) const
    {
        if (ioSupport() == IOSupport::SAVE) {
            throw std::runtime_error(
                "The action " + name() + " does not support loading images.");
        }
        // This should never be reached - the action declared that is able to
        // load images, but it does not implement the load method.
        assert(0);
        return Image();
    }

    /**
     * @brief Saves the given image to the given file.
     *
     * This member function must be implemented by the derived classes if the
     * @ref ioSupport member function returns IOSupport::SAVE or
     * IOSupport::BOTH.
     *
     * @param[in] filename: the file to write to
     * @param[in] image: the image to save
     * @param[in] log: the logger to use
     */
    virtual void save(
        const std::string& filename,
        const Image&       image,
        AbstractLogger&    log = logger()) const
    {
        if (ioSupport() == IOSupport::LOAD) {
            throw std::runtime_error(
                "The action " + name() + " does not support saving images.");
        }
        // This should never be reached - the action declared that is able to
        // save images, but it does not implement the save method.
        assert(0);
    }

    /* ************************************ *
     * Member functions already implemented *
     * ************************************ */

    Type type() const final { return Type::IMAGE_IO_ACTION; }

    MeshTypeId meshType() const final { return MeshTypeId::COUNT; }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IMAGE_IO_ACTION_H
