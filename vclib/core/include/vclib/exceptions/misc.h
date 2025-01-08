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

#ifndef VCL_EXCEPTIONS_MISC_H
#define VCL_EXCEPTIONS_MISC_H

#include <stdexcept>
#include <string>

namespace vcl {

/**
 * @brief Exception thrown when the size (generally of a container) is not the
 * expected one.
 *
 * @ingroup exceptions
 */
class WrongSizeException : public std::runtime_error
{
public:
    WrongSizeException(const std::string& err) : std::runtime_error(err) {}

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Wrong Size - ") + std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when the intersection between two objects is empty.
 *
 * @ingroup exceptions
 */
class NoIntersectionException : public std::runtime_error
{
public:
    NoIntersectionException(const std::string& err) : std::runtime_error(err) {}

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("No Intersection - ") + std::runtime_error::what();
        return error.c_str();
    }
};

} // namespace vcl

#endif // VCL_EXCEPTIONS_MISC_H
