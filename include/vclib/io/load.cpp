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

#include "load.h"

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType>
MeshType load(const std::string& filename, LogType& log, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return load<MeshType>(filename, loadedInfo, log, enableOptionalComponents);
}

template<MeshConcept MeshType, LoggerConcept LogType>
MeshType load(
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	LogType&           log,
	bool               enableOptionalComponents)
{
	MeshType m;
	load(m, filename, loadedInfo, log, enableOptionalComponents);
	return m;
}

template<MeshConcept MeshType, LoggerConcept LogType>
void load(MeshType& m, const std::string& filename, LogType& log, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	load(m, filename, loadedInfo, log, enableOptionalComponents);
}

template<MeshConcept MeshType, LoggerConcept LogType>
void load(
	MeshType&          m,
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	LogType&           log,
	bool               enableOptionalComponents)
{
	std::string ext = FileInfo::extension(filename);
	ext = vcl::str::toLower(ext);
	if (ext == ".obj") {
		io::loadObj(m, filename, loadedInfo, enableOptionalComponents);
	}
	else if (ext == ".off") {
		io::loadOff(m, filename, loadedInfo, enableOptionalComponents);
	}
	else if (ext == ".ply") {
		io::loadPly(m, filename, loadedInfo, enableOptionalComponents);
	}
	else if (ext == ".stl") {
		io::loadStl(m, filename, loadedInfo, log, enableOptionalComponents);
	}
	else {
		throw vcl::UnknownFileFormatException(ext);
	}
}

} // namespace vcl::io
