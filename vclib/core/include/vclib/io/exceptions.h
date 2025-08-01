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

#ifndef VCL_IO_EXCEPTIONS_H
#define VCL_IO_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace vcl {

/**
 * @brief Exception thrown when the file format is unknown.
 *
 * @ingroup exceptions
 */
class UnknownFileFormatException : public std::runtime_error
{
public:
    UnknownFileFormatException(const std::string& err) : std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error =
            std::string("Unknown File Format - ") + std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when the file cannot be opened.
 *
 * @ingroup exceptions
 */
class CannotOpenFileException : public std::runtime_error
{
public:
    CannotOpenFileException(const std::string& err) : std::runtime_error(err) {}

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Cannot Open File - ") + std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when the file is malformed.
 *
 * @ingroup exceptions
 */
class MalformedFileException : public std::runtime_error
{
public:
    MalformedFileException(const std::string& err) : std::runtime_error(err) {}

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Malformed File - ") + std::runtime_error::what();
        return error.c_str();
    }
};

} // namespace vcl

#endif // VCL_IO_EXCEPTIONS_H
