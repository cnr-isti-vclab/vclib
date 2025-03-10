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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_IMAGE_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_IMAGE_MANAGER_H

#include "io_action_container.h"

#include <vclib/processing/engine/action_interfaces.h>

namespace vcl::proc::detail {

class IOImageManager
{
    IOActionContainer<ImageIOAction> mImageIOActions;

protected:
    void add(const std::shared_ptr<ImageIOAction>& action)
    {
        mImageIOActions.add(action);
    }

public:
    // load image

    std::vector<FileFormat> loadImageFormats() const
    {
        return mImageIOActions.loadFormats();
    }

    std::shared_ptr<ImageIOAction> loadImageAction(FileFormat fmt) const
    {
        return mImageIOActions.loadAction(fmt);
    }

    // save image

    std::vector<FileFormat> saveImageFormats() const
    {
        return mImageIOActions.saveFormats();
    }

    std::shared_ptr<ImageIOAction> saveImageAction(FileFormat fmt) const
    {
        return mImageIOActions.saveAction(fmt);
    }
};

} // namespace vcl::proc::detail

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_IMAGE_MANAGER_H
