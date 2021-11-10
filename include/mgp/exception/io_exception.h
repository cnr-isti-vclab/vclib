/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_EXCEPTION_H
#define MGP_IO_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace mgp {

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

} // namespace mgp

#endif // MGP_IO_EXCEPTION_H
