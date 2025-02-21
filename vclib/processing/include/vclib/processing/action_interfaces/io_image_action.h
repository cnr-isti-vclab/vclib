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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_IO_IMAGE_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_IO_IMAGE_ACTION_H

#include <vclib/processing/action_interfaces/action.h>

#include <vclib/io/file_format.h>
#include <vclib/space/core/image.h>

#include <vector>

namespace vcl::proc {

class IOImageAction : public Action
{
public:
    enum class IOSupport {
        LOAD,
        SAVE,
        BOTH
    };

    Type type() const final { return Type::IO_IMAGE_ACTION; }

    /* *********************************************************** *
     * Members that must/may be implemented by the derived classes *
     * *********************************************************** */

    // From Action class

    virtual std::shared_ptr<Action> clone() const = 0;

    virtual std::string name() const = 0;

    /**
     * @brief Returns the type of support for input/output operations.
     *
     * Possible values are:
     * - LOAD: the action supports only loading images;
     * - SAVE: the action supports only saving images;
     * - BOTH: the action supports both loading and saving images.
     *
     * @return the type of support for input/output operations
     */
    virtual IOSupport ioSupport() const = 0;

    /**
     * @brief Returns the list of file formats supported by the action.
     *
     * Each file format is defined by a list of extensions (all the possible
     * extensions that a file format could have) and a description.
     *
     * @return the list of file formats supported by the action
     */
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
            return Image();
        }
        // This should never be reached - the action declared that is able to
        // load images, but it does not implement the load method.
        assert(0);
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
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_IO_IMAGE_ACTION_H
