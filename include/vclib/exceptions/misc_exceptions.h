/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_MISC_EXCEPTIONS_H
#define VCL_MISC_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace vcl {

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

#endif // VCL_MISC_EXCEPTIONS_H
