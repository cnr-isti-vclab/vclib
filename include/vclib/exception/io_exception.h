/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_IO_EXCEPTION_H
#define VCL_IO_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace vcl {

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

#endif // VCL_IO_EXCEPTION_H
