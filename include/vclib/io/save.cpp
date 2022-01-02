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

#include "save.h"

namespace vcl::io {

template<typename MeshType>
void save(const MeshType& m, const std::string& filename, bool binary)
{
	vcl::io::FileMeshInfo info(m);
	save(m, filename, info, binary);
}

template<typename MeshType>
void save(const MeshType& m, const std::string& filename, const FileMeshInfo& info, bool binary)
{
	std::string name, ext;
	vcl::fileInfo::separateExtensionFromFilename(filename, name, ext);
	ext = vcl::str::toLower(ext);
	if (ext == ".obj") {
		saveObj(m, filename, info);
	}
	else if (ext == ".off") {
		saveOff(m, filename, info);
	}
	else if (ext == ".ply") {
		savePly(m, filename, info, binary);
	}
	else {
		throw vcl::UnknownFileFormatException(ext);
	}
}

} // namespace vcl::io
