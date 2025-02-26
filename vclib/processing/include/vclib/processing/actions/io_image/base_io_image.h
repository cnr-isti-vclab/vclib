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

#ifndef VCL_PROCESSING_ACTIONS_IO_IMAGE_BASE_IO_IMAGE_H
#define VCL_PROCESSING_ACTIONS_IO_IMAGE_BASE_IO_IMAGE_H

#include <vclib/processing/engine.h>

namespace vcl::proc {

class BaseIOImage : public ImageIOAction
{
public:
    std::string name() const final { return "Base IO Image"; }

    std::shared_ptr<Action> clone() const final
    {
        return std::make_shared<BaseIOImage>(*this);
    }

    IOSupport ioSupport() const final { return IOSupport::BOTH; }

    std::vector<FileFormat> supportedFormats() const final
    {
        std::vector<FileFormat> formats;
        formats.push_back(FileFormat("png", "Portable Network Graphics"));
        formats.push_back(FileFormat("bmp", "Bitmap"));
        formats.push_back(FileFormat("tga", "Truevision TGA"));
        formats.push_back(FileFormat(
            std::vector<std::string> {"jpg", "jpeg"},
            "Joint Photographic Experts Group"));

        return formats;
    }

    Image load(const std::string& filename, AbstractLogger& log = logger())
        const final
    {
        Image img(filename);
        if (img.isNull()) {
            throw std::runtime_error("Error loading image from " + filename);
        }
        return img;
    }

    void save(
        const std::string& filename,
        const Image&       image,
        AbstractLogger&    log = logger()) const final
    {
        assert(!image.isNull());
        image.save(filename);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_IO_IMAGE_BASE_IO_IMAGE_H
