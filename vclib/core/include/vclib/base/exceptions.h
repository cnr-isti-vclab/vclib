// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_EXCEPTIONS_H
#define VCL_BASE_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace vcl {

/**
 * @defgroup exceptions Exceptions
 *
 * @brief List of exceptions used in the library.
 */

/**
 * @brief Exception thrown when the size (generally of a container) is not the
 * expected one.
 *
 * @ingroup base
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

} // namespace vcl

#endif // VCL_BASE_EXCEPTIONS_H
