// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/base/exceptions.h>

#include <vclib/base.h>
#include <vclib/io.h>
#include <vclib/mesh.h>

namespace vcl::bind {

void initBaseExceptions(pybind11::module& m)
{
    namespace py = pybind11;

    py::register_exception<WrongSizeException>(m, "WrongSizeException");
    py::register_exception<MissingComponentException>(
        m, "MissingComponentException");
    py::register_exception<MissingPreconditionException>(
        m, "MissingPreconditionException");
    py::register_exception<MissingCompactnessException>(
        m, "MissingCompactnessException");
    py::register_exception<InconsistentMeshException>(
        m, "InconsistentMeshException");
    py::register_exception<MissingTriangularRequirementException>(
        m, "MissingTriangularRequirementException");
    py::register_exception<MissingQuadRequirementException>(
        m, "MissingQuadRequirementException");
    py::register_exception<BadVertexIndexException>(
        m, "BadVertexIndexException");
    py::register_exception<BadCustomComponentTypeException>(
        m, "BadCustomComponentTypeException");
    py::register_exception<UnknownFileFormatException>(
        m, "UnknownFileFormatException");
    py::register_exception<CannotOpenFileException>(
        m, "CannotOpenFileException");
    py::register_exception<MalformedFileException>(m, "MalformedFileException");
}

} // namespace vcl::bind
