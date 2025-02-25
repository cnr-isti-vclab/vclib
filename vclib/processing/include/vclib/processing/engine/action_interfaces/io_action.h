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

#ifndef VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IO_ACTION_H
#define VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IO_ACTION_H

#include "action.h"

#include <vclib/io/file_format.h>

namespace vcl::proc {

class IOAction : public Action
{
public:
    enum class IOSupport {
        LOAD,
        SAVE,
        BOTH
    };

    /* ******************************************************************** *
     * Member functions that must/may be implemented by the derived classes *
     * ******************************************************************** */

    // From Action class

    [[nodiscard]] virtual std::shared_ptr<Action> clone() const = 0;

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
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_ACTION_INTERFACES_IO_ACTION_H
