/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_EXCEPTION_H
#define MGP_MESH_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace mgp {

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

class MissingPropertyException : public std::runtime_error
{
public:
	MissingPropertyException(const std::string& err) : std::runtime_error(err) {}
	virtual const char* what() const throw()
	{
		static std::string error;
		error = std::string("Missing Property - ") + std::runtime_error::what();
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

} // namespace mgp

#endif // MGP_MESH_EXCEPTION_H
