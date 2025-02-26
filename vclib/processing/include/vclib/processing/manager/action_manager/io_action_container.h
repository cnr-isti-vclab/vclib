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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_ACTION_CONTAINER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_ACTION_CONTAINER_H

#include <vclib/io/file_format.h>
#include <vclib/processing/engine/action_interfaces/io_action.h>
#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include <map>

namespace vcl::proc {

class IOActionContainer
{
    using FormatMap = std::map<FileFormat, std::shared_ptr<IOAction>>;

    FormatMap mLoadFormatMap;
    FormatMap mSaveFormatMap;

public:
    IOActionContainer() = default;

    void add(std::shared_ptr<IOAction> action)
    {
        using enum IOAction::IOSupport;

        if (!action) {
            throw std::runtime_error("Action is nullptr.");
        }
        std::vector<FileFormat> formats = action->supportedFormats();

        for (const auto& format : formats) {
            checkFormatDoesNotExist(format);
            auto supp = action->ioSupport();
            if (supp == LOAD || supp == BOTH) {
                mLoadFormatMap[format] =
                    std::dynamic_pointer_cast<IOAction>(action);
            }
            if (supp == SAVE || supp == BOTH) {
                mSaveFormatMap[format] =
                    std::dynamic_pointer_cast<IOAction>(action);
            }
        }
    }

    std::shared_ptr<IOAction> loadAction(const FileFormat& format) const
    {
        auto it = findLoadFormatExists(format);
        return it->second;
    }

    std::vector<FileFormat> loadFormats() const
    {
        std::vector<FileFormat> formats;
        for (const auto& [format, _] : mLoadFormatMap) {
            formats.push_back(format);
        }
        return formats;
    }

    std::shared_ptr<IOAction> saveAction(const FileFormat& format) const
    {
        auto it = findSaveFormatExists(format);
        return it->second;
    }

    std::vector<FileFormat> saveFormats() const
    {
        std::vector<FileFormat> formats;
        for (const auto& [format, _] : mSaveFormatMap) {
            formats.push_back(format);
        }
        return formats;
    }

private:
    void checkFormatDoesNotExist(const FileFormat& format) const
    {
        if (mLoadFormatMap.find(format) != mLoadFormatMap.end()) {
            throw std::runtime_error("Format already registered for loading.");
        }
        if (mSaveFormatMap.find(format) != mSaveFormatMap.end()) {
            throw std::runtime_error("Format already registered for saving.");
        }
    }

    FormatMap::const_iterator findLoadFormatExists(
        const FileFormat& format) const
    {
        auto it = mLoadFormatMap.find(format);
        if (it == mLoadFormatMap.end()) {
            throw std::runtime_error("Format not registered for loading.");
        }
        return it;
    }

    FormatMap::const_iterator findSaveFormatExists(
        const FileFormat& format) const
    {
        auto it = mSaveFormatMap.find(format);
        if (it == mSaveFormatMap.end()) {
            throw std::runtime_error("Format not registered for saving.");
        }
        return it;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_ACTION_CONTAINER_H
