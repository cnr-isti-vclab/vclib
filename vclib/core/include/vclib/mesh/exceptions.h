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

#ifndef VCL_MESH_EXCEPTIONS_H
#define VCL_MESH_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace vcl {

/**
 * @brief Exception thrown when the mesh is inconsistent.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class InconsistentMeshException : public std::runtime_error
{
public:
    InconsistentMeshException(const std::string& err) : std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error =
            std::string("Inconsistent Mesh - ") + std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when a precondition on an input/output mesh is
 * missing.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class MissingPreconditionException : public std::runtime_error
{
public:
    MissingPreconditionException(const std::string& err) :
            std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Missing Mesh Precondition - ") +
                std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when the mesh is not compact.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class MissingCompactnessException : public std::runtime_error
{
public:
    MissingCompactnessException(const std::string& err) :
            std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error =
            std::string("Lack of Compactness - ") + std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when a mesh/element component is missing (not
 * enabled).
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class MissingComponentException : public std::runtime_error
{
public:
    MissingComponentException(const std::string& err) : std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error =
            std::string("Missing Component - ") + std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when an input/output mesh is not composed of
 * triangles.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class MissingTriangularRequirementException : public std::runtime_error
{
public:
    MissingTriangularRequirementException(const std::string& err) :
            std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Missing Triangular Mesh Requirement - ") +
                std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when an input/output mesh is not composed of quads.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class MissingQuadRequirementException : public std::runtime_error
{
public:
    MissingQuadRequirementException(const std::string& err) :
            std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Missing Quad Mesh Requirement - ") +
                std::runtime_error::what();
        return error.c_str();
    }
};

/**
 * @brief Exception thrown when an index is out of bounds in a vertex container.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
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

/**
 * @brief Exception thrown when the type of a custom component is not the one
 * expected.
 *
 * @ingroup exceptions
 * @ingroup mesh
 */
class BadCustomComponentTypeException : public std::runtime_error
{
public:
    BadCustomComponentTypeException(const std::string& err) :
            std::runtime_error(err)
    {
    }

    virtual const char* what() const throw()
    {
        static std::string error;
        error = std::string("Bad Custom Component Type - ") +
                std::runtime_error::what();
        return error.c_str();
    }
};

} // namespace vcl

#endif // VCL_MESH_EXCEPTIONS_H
