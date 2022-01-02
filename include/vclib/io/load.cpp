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

#include "load.h"

namespace vcl::io {

template<typename MeshType>
MeshType load(const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return load<MeshType>(filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
MeshType load(const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents)
{
	MeshType m;
	load(m, filename, loadedInfo, enableOptionalComponents);
	return m;
}

template<typename MeshType>
void load(MeshType& m, const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	load(m, filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
void load(
	MeshType&          m,
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	bool               enableOptionalComponents)
{
	std::string name, ext;
	vcl::fileInfo::separateExtensionFromFilename(filename, name, ext);
	ext = vcl::str::toLower(ext);
	if (ext == ".obj") {
		loadObj(m, filename, loadedInfo, enableOptionalComponents);
	}
	else if (ext == ".off") {
		loadOff(m, filename, loadedInfo, enableOptionalComponents);
	}
	else if (ext == ".ply") {
		loadPly(m, filename, loadedInfo, enableOptionalComponents);
	}
	else {
		throw vcl::UnknownFileFormatException(ext);
	}
}

} // namespace vcl::io
