// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
