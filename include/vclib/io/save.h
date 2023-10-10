/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_IO_SAVE_H
#define VCL_IO_SAVE_H

#include "obj/save.h"
#include "off/save.h"
#include "ply/save.h"
#include "stl/save.h"

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void save(
	const MeshType&    m,
	const std::string& filename,
	LogType&           log    = nullLogger,
	bool               binary = true)
{
	MeshInfo info(m);
	save(m, filename, info, log, binary);
}

template<MeshConcept MeshType>
void save(const MeshType& m, const std::string& filename, bool binary)
{
	MeshInfo info(m);
	NullLogger log;
	save(m, filename, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void save(
	const MeshType&    m,
	const std::string& filename,
	const MeshInfo&    info,
	LogType&           log    = nullLogger,
	bool               binary = true)
{
	std::string ext = FileInfo::extension(filename);
	ext = vcl::str::toLower(ext);
	if (ext == ".obj") {
		io::saveObj(m, filename, info, log);
	}
	else if (ext == ".off") {
		io::saveOff(m, filename, info, log);
	}
	else if (ext == ".ply") {
		io::savePly(m, filename, info, log, binary);
	}
	else if (ext == ".stl") {
		io::saveStl(m, filename, info, log, binary);
	}
	else {
		throw vcl::UnknownFileFormatException(ext);
	}
}

} // namespace vcl

#endif // VCL_IO_SAVE_H
