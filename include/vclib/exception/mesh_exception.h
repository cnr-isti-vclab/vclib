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

#ifndef VCL_MESH_EXCEPTION_H
#define VCL_MESH_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace vcl {

class MissingPreconditionException : public std::runtime_error
{
public:
	MissingPreconditionException(const std::string& err) : std::runtime_error(err) {}

	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Missing Mesh Precondition - ") + std::runtime_error::what();
		return error.c_str();
	}
};

class MissingCompactnessException : public std::runtime_error
{
public:
	MissingCompactnessException(const std::string& err) : std::runtime_error(err) {}
	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Lack of Compactness - ") + std::runtime_error::what();
		return error.c_str();
	}
};

class MissingComponentException : public std::runtime_error
{
public:
	MissingComponentException(const std::string& err) : std::runtime_error(err) {}
	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Missing Component - ") + std::runtime_error::what();
		return error.c_str();
	}
};

class MissingTriangularRequirementException : public std::runtime_error
{
public:
	MissingTriangularRequirementException(const std::string& err) : std::runtime_error(err) {}

	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Missing Triangular Mesh Requirement - ") + std::runtime_error::what();
		return error.c_str();
	}
};

class MissingQuadRequirementException : public std::runtime_error
{
public:
	MissingQuadRequirementException(const std::string& err) : std::runtime_error(err) {}

	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Missing Quad Mesh Requirement - ") + std::runtime_error::what();
		return error.c_str();
	}
};

class MissingPolygonalRequirementException : public std::runtime_error
{
public:
	MissingPolygonalRequirementException(const std::string& err) : std::runtime_error(err) {}

	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Missing Polygonal Mesh Requirement - ") + std::runtime_error::what();
		return error.c_str();
	}
};

class BadVertexIndexException : public std::runtime_error
{
public:
	BadVertexIndexException(const std::string& err) : std::runtime_error(err) {}

	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Bad Vertex Index - ") + std::runtime_error::what();
		return error.c_str();
	}
};

} // namespace vcl

#endif // VCL_MESH_EXCEPTION_H
