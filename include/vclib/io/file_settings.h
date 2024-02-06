/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_IO_FILE_SETTINGS_H
#define VCL_IO_FILE_SETTINGS_H

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for
// header only
#include "../../../external/nlohmann-json-3.11.2/single_include/nlohmann/json.hpp"
#endif

#include <fstream>

#include <vclib/exceptions/io_exceptions.h>

namespace vcl {

class FileSettings
{
    using json = nlohmann::json;

    json settings;

public:
    FileSettings() = default;
    FileSettings(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
            throw vcl::CannotOpenFileException(filename);
        settings = json::parse(file);
    }

    void save(const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
            throw vcl::CannotOpenFileException(filename);
        file << settings.dump();
    }
};

} // namespace vcl

#endif // VCL_IO_FILE_SETTINGS_H
